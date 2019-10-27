#include "include/utf8_tools.h"
#include "include/utf8.h"
#include <stdint.h>
#include <string>
#include <vector>

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


std::string utf8_substr(const std::string &s, size_t st, size_t en) {
    if (en > 0 && st > en) {
        throw std::out_of_range("Start index is bigger than end!");
    }

    std::string result;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;
    size_t counter = 0;
    while (begin != end) {
        if (counter < st) {
            utf8::next(begin, end);
        } else if (en == 0 || counter + 1 < en) {
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

std::string utf8_repeat(const std::string &s, size_t n) {
    std::string result;
    for (size_t c = 1; c <= n; c++) {
        result.append(s);
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
