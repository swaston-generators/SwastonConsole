#ifndef UTF8_TOOLS_H
#define UTF8_TOOLS_H

#include "utf8_tools.h"
#include "utf8.h"

#include <stdint.h>
#include <stdlib.h>

#include <string>
#include <vector>

#ifdef _WIN32

#include <io.h>
#include <fcntl.h>

#endif

std::string utf8_join(const std::string &s, const std::string &separator) {
    std::string result;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;

    while (true) {
        cp = utf8::next(begin, end);
        utf8::append(cp, std::back_inserter(result));

        if (begin != end) {
            result.append(std::string(separator));
        } else {
            break;
        }

    }
    return result;

}

std::string utf8_reverse(const std::string &s) {
    std::string result;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;

    while (begin != end) {
        cp = utf8::prior(end, begin);
        utf8::append(cp, std::back_inserter(result));
    }

    return result;
}


std::string utf8_substr(const std::string &s, size_t st, size_t en = 0) {
    if (st == 0 && en == 0) {
        return std::string();
    }
    if (en > 0 && st > en) {
        throw std::out_of_range("Start index is bigger than end");
    }

    size_t distance = utf8::distance(s.begin(), s.end());

    if (st > distance || en > distance) {
        throw std::out_of_range("Index is greater than the string length");
    }

    std::string result;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;
    size_t counter = 0;

    for (; counter < st; counter++) { // skip string up to start value
        utf8::next(begin, end);
    }
    while (begin != end) {
        if (en == 0 || counter + 1 < en) {
            cp = utf8::next(begin, end);
            utf8::append(cp, std::back_inserter(result));
        } else if (counter == en) {
            cp = utf8::next(begin, end);
            utf8::append(cp, std::back_inserter(result));
            break;
        }
        counter++;

    }
    return result;
}

std::vector<std::string> utf8_to_vector(const std::string &s) {
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();

    std::vector<std::string> vec;

    uint32_t cp = 0;
    while (begin != end) {
        cp = utf8::next(begin, end);
        std::string result;
        utf8::append(cp, std::back_inserter(result));
        vec.push_back(result);
    }
    return vec;
}


std::wstring utf8string_to_utf16_wstring(const std::string &s) {
    std::wstring result;
    utf8::utf8to16(s.begin(), s.end(), std::back_inserter(result));
    return result;
}

std::string utf16wstring_to_utf8string(const std::wstring &ws) {
    std::string result;
    utf8::utf16to8(ws.begin(), ws.end(), std::back_inserter(result));
    return result;
}


void utf8_print(const std::string &s) {
#ifdef _WIN32
    // don't use this function along with standard ones, they'll try to use standard mode _O_TEXT - crash on windows
    int mode = _setmode(_fileno(stdout), _O_U16TEXT); // isn't thread safe, consider using mutex
    if (mode == -1) {
        throw std::runtime_error("Cannot set output mode to UTF-16");
    }
    fwprintf(stdout, L"%s", utf8string_to_utf16_wstring(s).c_str());
    mode = _setmode(_fileno(stdout), mode);

    if (mode == -1) {
        throw std::runtime_error("Cannot revert output mode");
    }
#else
    fprintf(stdout, "%s", s.c_str());
#endif
}

std::string utf8_getstring() {
#ifdef _WIN32 // platform-specific code
#define platform_char wchar_t
#define end_of_file WEOF
#define strtype(s) L ## s
#define platform_int  wint_t
#define unigetchar() getwchar()

    int mode = _setmode(_fileno(stdin), _O_U16TEXT);
    if (mode == -1) {
        throw std::runtime_error("Cannot set input mode to UTF-16");
    }

#else
#define platform_char char
#define end_of_file EOF
#define strtype(s) s
#define platform_int  int
#define unigetchar() getchar()

#endif // ~platform-specific code
    const size_t buf_size = 8;
    platform_char *p_str = (platform_char *) (malloc(sizeof(platform_char) * buf_size));
    if (p_str == NULL) {
        throw std::bad_alloc();
    }

    size_t
            current_size = buf_size,
            i = 0;
    platform_int tmp_int; // wint_t if windows, else int


    while (true) {
        tmp_int = unigetchar();
        if (tmp_int == strtype('\n') ||
            (tmp_int == end_of_file)) {
            break;
        }
        p_str[i] = (platform_char) tmp_int;

        // if i reached maximize size - realloc size
        if (current_size == ++i) {
            current_size = i + buf_size;
            platform_char *mem = (platform_char *) realloc(p_str, sizeof(platform_char) * current_size);

            if (mem == NULL) {
                free(p_str);
                throw std::bad_alloc();
            }
            p_str = mem;
        }

    }

    p_str[i] = strtype('\0');
#ifdef _WIN32
    mode = _setmode(_fileno(stdin), mode);
    if (mode == -1) {
        throw std::runtime_error("Cannot revert input mode");
    }
    std::string s(utf16wstring_to_utf8string((platform_char *) p_str));
#else
    std::string s((platform_char*)p_str);

#endif
//free it
    free(p_str);
    return s;

}


#endif
