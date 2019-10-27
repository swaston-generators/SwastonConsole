#include "include/utf8.h"
#include "include/swaston_generator.h"
#include "include/utf8_tools.h"

#include <stdio.h>
#include <cstdlib>
#include <string>

#ifdef _WIN32

#include <io.h>
#include <fcntl.h>

#pragma warning(disable : 4996) // needed to compile non-buffer-overflow-aware c functions
#define sep '\\'
#else
#define sep '/'
#endif


void utf8_print(const std::string &s) {
#ifdef _WIN32
    int mode = _setmode(_fileno(stdout), _O_U16TEXT);
    if (mode == -1) {
        throw std::runtime_error("Cannot set output mode to UTF-16");
    }
    fwprintf(stdout, L"%s", utf8string_to_utf16_wstring(s).c_str());
#else
    fprintf(stdout, "%s", s.c_str());
#endif
}

std::string utf8_getstring() {
#ifndef _WIN32 // platform-specific code
#define chartype char
#define end_of_file EOF
#define strtype(s) s
#define inttype  int
#define unigetchar() getchar()
#else
#define chartype wchar_t
#define end_of_file WEOF
#define strtype(s) L ## s
#define inttype  wint_t
#define unigetchar() getwchar()

    int mode = _setmode(_fileno(stdin), _O_U16TEXT);
    if (mode == -1) {
        throw std::runtime_error("Cannot set input mode to UTF-16");
    }

#endif // ~platform-specific code
    size_t buf_size = 128;
    size_t current_size = 0;

    chartype *p_str = (chartype *) (malloc(sizeof(chartype) * buf_size));
    current_size = buf_size;


    if (p_str == NULL) {
        throw std::bad_alloc();
    }
    inttype in; // wint_t if windows, else int
    chartype ch; // wchar_t if windows, else char
    size_t i = 0;

    while (true) {
        in = unigetchar();
        if (in == strtype('\n') ||  // unigetchar - getwchar if windows, else getchar
            (in == end_of_file)) {
            break;
        }
        ch = in;
        p_str[i] = (chartype) ch;
        i++;

        // if i reached maximize size then realloc size
        if (i == current_size) {
            current_size = i + buf_size;
            p_str = (chartype *) realloc(p_str, sizeof(chartype) * current_size);
        }
    }


    p_str[i] = strtype('\0');
#ifndef _WIN32
    std::string s((chartype*)p_str);
#else
    std::string s(utf16wstring_to_utf8string((chartype *) p_str));
#endif
//free it
    free(p_str);
    return
            s;

}


#ifndef _WIN32
int main(int argc, char **argv) {
    std::vector<std::string> vec_argv;
    vec_argv.reserve(argc);
    for(int i=0; i< argc; i++){
        vec_argv.push_back((std::string)argv[i]);
    }
#else

//wmain is used on windows because char argv can't take utf-16 input (broken string as input otherwise)
int wmain(int argc, wchar_t **argv) {
    std::vector<std::string> vec_argv;
    vec_argv.reserve(argc);
    for (int i = 0; i < argc; i++) { // create vector of utf-8 strings
        vec_argv.push_back(utf16wstring_to_utf8string(argv[i]));
    }
#endif

    std::string input;
    if (argc == 1) { // case no input
        fprintf(stdout, "input: ");
        try {
            input = utf8_getstring();
        }
        catch (std::runtime_error) {
            fprintf(stdout, "Cannot set input mode to UTF-16");
            exit(-1);
        }
        catch (std::bad_alloc) {
            fprintf(stdout, "Cannot allocate needed memory");
            exit(-1);
        }

    } else if (
            argc > 2 ||
            (argc == 2 &&
             (
                     vec_argv[1] == "--help" ||
                     vec_argv[1] == "-h"))
            ) { // case too many arguments or help cmd
        std::string name(vec_argv[0]);
        utf8_print("Usage: " + utf8_substr(name, name.rfind(sep) + 1) + " [word]\n");
        exit(0);
    } else { // case argv input
        input.append(vec_argv[1]);
        if (!utf8::is_valid(input.begin(), input.end())) {
            std::string result;
            utf8::utf16to8(input.begin(), input.end(), std::back_inserter(result));
            input = result;
        }
    }
    size_t dist = utf8::distance(input.begin(), input.end());
    if (dist == 0) { // input is empty (instant EOF or eol)
        exit(0);
    } else if (dist == 1) { // input takes only one utf-8 char, repeat it 3 times
        input = utf8_repeat(input, 3);
    }
    try {
        utf8_print(generate_swaston(input));
    }
    catch (std::runtime_error) {
        fprintf(stdout, "Cannot set output mode to UTF-16");
        exit(-1);
    }

    return 0;
}