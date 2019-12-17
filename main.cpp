#include "include/utf8.h"
#include "include/swaston_generator.h"

#include <stdio.h>
#include <string>

#ifdef _WIN32
#define sep '\\'
#else
#define sep '/'
#endif

#ifdef _WIN32

// wmain is used on windows because char argv can't take utf-16 input (broken *char as input otherwise)
int wmain(int argc, wchar_t **wargv) {
    std::vector<std::string> vec_argv;
    vec_argv.reserve(argc);
    for (int i = 0; i < argc; i++) {
        vec_argv.push_back(utf16wstring_to_utf8string(wargv[i]));
    }
#else
int main(int argc, char **argv) {
     std::vector<std::string> vec_argv;
     vec_argv.reserve(argc);
     for(int i=0; i< argc; i++){
      vec_argv.push_back((std::string)argv[i]);
     }
#endif

    std::string input;
    const std::string APP_NAME(vec_argv[0]);
    const std::string USAGE_TEXT("Usage: " + APP_NAME.substr(APP_NAME.rfind(sep)) + " [word]\n");
    if (argc == 1) {
        // case no input
        fprintf(stdout, "input: ");
        try {
            while (true) {
                input = utf8_getstring();
                if (!input.empty()) { break; };
            }
        }
        catch (std::runtime_error) {
            fprintf(stdout, "Cannot set input mode to UTF-16");
            exit(EXIT_FAILURE);
        }
        catch (std::bad_alloc) {
            fprintf(stdout, "Cannot allocate needed memory");
            exit(EXIT_FAILURE);
        }

    } else if (argc > 2 || (argc == 2 && (vec_argv[1] == "--help" || vec_argv[1] == "-h"))) {
        // case too many arguments(not "" enclosed) or help cmd as input
        utf8_print(USAGE_TEXT);
        exit(EXIT_SUCCESS);
    } else { // case argv input
        input.append(vec_argv[1]);
        if (!utf8::is_valid(input.begin(), input.end())) {
            std::string result;
            utf8::utf16to8(input.begin(), input.end(), std::back_inserter(result));
            input = result;
        }
    }
    size_t dist = utf8::distance(input.begin(), input.end());
    if (dist == 0) {
        // input is empty (instant EOF or eol)
        utf8_print(USAGE_TEXT);
        exit(EXIT_SUCCESS);
    } else if (dist == 1) {
        // input takes only one utf-8 char, repeat it 3 times
        input = utf8_repeat(input, 3);
    }
    try {
        utf8_print(generate_swaston(input));
    }
    catch (std::runtime_error) {
        fprintf(stdout, "Cannot set output mode to UTF-16");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}