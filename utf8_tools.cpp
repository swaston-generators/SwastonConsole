#include "include/utf8_tools.h"
#include "include/utf8.h"
#include <stdint.h>
#include <string>
#include <vector>

std::string utf8_join(const std::string &s, const std::string &separator) {
    std::string back;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;

    while (true) {
        cp = utf8::next(begin, end);
        utf8::append(cp, std::back_inserter(back));

        if (begin != end) {
            back.append(separator);
        } else {
            break;
        }

    }
    return back;

}

std::string utf8_reverse(const std::string &s) {
    std::string back;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;

    while (begin != end) {
        cp = utf8::prior(end, begin);
        utf8::append(cp, std::back_inserter(back));
    }

    return back;

}


std::string utf8_substr(const std::string &s, int st, int en) {

    if (en > 0 && st > en) {
        throw std::out_of_range("Start index is bigger than end!");
    }

    std::string back;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();
    uint32_t cp = 0;
    int counter = 0;
    while (begin != end) {
        if (counter < st) {
            cp = utf8::next(begin, end);
        } else if (en == 0 || counter < en - 1) {

            cp = utf8::next(begin, end);
            utf8::append(cp, std::back_inserter(back));
        } else if (counter == en) {
            cp = utf8::next(begin, end);
            utf8::append(cp, std::back_inserter(back));
            break;
        }
        counter++;

    }
    return back;
}


std::string utf8_repeat(const std::string &s, int n) {
    std::string temp;
    for (int c = 1; c <= n; c++) {
        temp.append(s);
    }
    return temp;
}

std::vector<std::string> utf8_to_vector(const std::string &s) {
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator end = s.end();

    std::vector<std::string> vec;

    uint32_t cp = 0;
    while (begin != end) {
        cp = utf8::next(begin, end);
        std::string back;
        utf8::append(cp, std::back_inserter(back));
        vec.push_back(back);
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
