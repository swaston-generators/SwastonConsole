#ifndef utf8_tools
#define utf8_tools

#include <string>
#include <vector>

std::string utf8_join(const std::string &, const std::string & = ", ");

std::string utf8_reverse(const std::string &);

std::string utf8_substr(const std::string &, size_t, size_t = 0);

std::string utf8_repeat(const std::string &, size_t);

std::vector<std::string> utf8_to_vector(const std::string &);

std::wstring utf8string_to_utf16_wstring(const std::string &);

std::string utf16wstring_to_utf8string(const std::wstring &);

#endif
