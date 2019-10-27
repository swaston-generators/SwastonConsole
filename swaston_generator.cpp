#include "include/swaston_generator.h"
#include "include/utf8.h"
#include "include/utf8_tools.h"
#include <string>

std::string generate_swaston(const std::string &s) {
    std::string
            l_spaced_word = utf8_join(s, " "),
            r_spaced_word = utf8_reverse(utf8_join(s, " ")),
            l_word = utf8_reverse(s),
    //r_word = s
            center = l_spaced_word + utf8_substr(r_spaced_word, 1) + '\n',
            upper,
            lower,
            final,
            t = utf8_repeat(" ", utf8::distance(r_spaced_word.begin(), r_spaced_word.end()) - size_t(2));

    std::vector<std::string>
            r_vec = utf8_to_vector(s),
            l_vec = utf8_to_vector(l_word); // convert utf-8 string to vector of strings

    size_t size = l_vec.size();

    for (size_t c = 0; c != size; c++) { // upper side

        if (c == 0) { // case first row
            upper.append(l_vec[c] + t + l_spaced_word + '\n');
        } else if (c != size - size_t(1)) {
            upper.append(l_vec[c] + t + r_vec[c] + t + '\n');

        }
    }

    for (size_t c = 0; c != size; c++) { // lower side
        if (c == 0) { // case first row (center)
            continue;
        } else if (c != size - size_t(1)) {
            lower.append(t + ' ' + l_vec[c] + t + r_vec[c] + '\n');
        } else { // case last row
            lower.append(r_spaced_word + t + r_vec[c] + '\n');
        }
    }

    final.append(upper);
    final.append(center);
    final.append(lower);
    return final;
}
