#include "utf8.h"
#include "swaston_generator.h"
#include "utf8_tools.h"

#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#pragma warning(disable : 4996)
#define sep "\\"
#else
#define sep "/"
#endif


void utf8_printf(const std::string& s) {
#ifdef _WIN32
	int mode = _setmode(_fileno(stdout), _O_U16TEXT);
	if (mode == -1) {
		fprintf(stdout, "Cannot set output mode to UTF-16");
		exit(-1);
	}
	fwprintf(stdout, utf8string_to_utf16_wstring(s).c_str());
#else
	fprintf(stdout, "%s", s.c_str());
#endif
}

std::string utf8_scanf() {
	size_t buf = 100;
#ifdef _WIN32
	int mode = _setmode(_fileno(stdin), _O_U16TEXT);
	if (mode == -1) {
		fprintf(stdout, "Cannot set input mode to UTF-16");
		exit(-1);
	}
	void *input = malloc(sizeof(wchar_t) * buf);
	
	if (input == NULL){
        free(input);
		fprintf(stdout, "Cannot allocate memory");
		exit(-1);
	}

    if (fwscanf(stdin, L"%100s", (wchar_t*)input) != 1) {
        free(input);
        fprintf(stdout, "Cannot read input values");
        exit(-1);
    }
	std::string s(utf16wstring_to_utf8string((wchar_t *)input));
#else
	void *input = malloc(sizeof(char) * buf);
	if (input == NULL || fscanf(stdin, "%100s", (char*)input) != 1) {
		fprintf(stdout, "Cannot read input values");
		exit(-1);
	}
	std::string s((char *)input);
#endif
	free(input);
	if (!utf8::is_valid(s.begin(), s.end())) {
		std::string temp;
		utf8::utf16to8(s.begin(), s.end(), std::back_inserter(temp));
		s = temp;
	}
	return s;
}

int main(int argc, char** argv) {
	std::string input;
	if (argc == 1) {
		fprintf(stdout, "input: ");
		input = utf8_scanf();
	}
	else if (argc != 2 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
		std::string tmp(argv[0]);
		fprintf(stdout, "Usage: %s [word]\n", tmp.substr(tmp.rfind(sep) + 1).c_str());
		exit(0);
	}
	else {
		input = argv[1];
		if (!utf8::is_valid(input.begin(), input.end())) {
			std::string temp;
			utf8::utf16to8(input.begin(), input.end(), std::back_inserter(temp));
			input = temp;
		}
	}
	
	if(utf8::distance(input.begin(), input.end()) < 2){
		input = utf8_repeat(input, 3);
    }
	utf8_printf(generate_swaston(input));
	return 0;
}
