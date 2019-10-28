#include "include/swaston_generator.h"
#include "include/utf8.h"
#include "include/utf8_tools.h"
#include <string>

std::string generate_swaston(const std::string &s) {
    if (utf8::distance(s.begin(), s.end()) < 2) {
        return s;
    }
    std::string
            l_spaced_word = utf8_join(s, " "),
            r_spaced_word = utf8_reverse(utf8_join(s, " ")),
            l_word = utf8_reverse(s),
    //r_word = s
            center = l_spaced_word + utf8_substr(r_spaced_word, 1) + '\n',
            t = utf8_repeat(" ", utf8::distance(r_spaced_word.begin(), r_spaced_word.end()) - size_t(2)),
            upper,
            lower,
            final;

    std::vector<std::string>
            r_vec = utf8_to_vector(s),
            l_vec = utf8_to_vector(l_word); // convert utf-8 string to vector of strings

    size_t size = l_vec.size();

    for (size_t c = 0; c != size; c++) { // upper side

        if (c == 0) { // case first row
            upper += l_vec[c];
            upper += t;
            upper += l_spaced_word;
            upper += '\n';
        } else if (c + 1 != size) {
            upper += l_vec[c];
            upper += t;
            upper += r_vec[c];
            upper += t;
            upper += '\n';

        }
    }

    for (size_t c = 0; c != size; c++) { // lower side
        if (c == 0) { // case first row (center)
            continue;
        } else if (c + 1 != size) {
            lower += t;
            lower += ' ';
            lower += l_vec[c];
            lower += t;
            lower += r_vec[c];
            lower += '\n';
        } else { // case last row
            lower += r_spaced_word;
            lower += t;
            lower += r_vec[c];
            lower += '\n';
        }
    }

    final.append(upper);
    final.append(center);
    final.append(lower);
    return final;
}
