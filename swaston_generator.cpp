#include "swaston_generator.h"

#include "utf8.h"
#include "utf8_tools.h"

std::string generate_swaston(const std::string& s) {
	std::string
		l_spaced_word = utf8_join(s, " "),
		r_spaced_word = utf8_reverse(utf8_join(s, " ")),
		l_word = utf8_reverse(s),
		r_word = s,
		center = l_spaced_word + utf8_substr(r_spaced_word, 1) + '\n',
		upper,
		lower,
		final,
		t = utf8_repeat(" ", utf8::distance(r_spaced_word.begin(), r_spaced_word.end()) - size_t(2));

	std::vector<std::string>
		r_vec = utf8_to_vector(r_word),
		l_vec = utf8_to_vector(l_word);

	size_t size = utf8::distance(l_word.begin(), l_word.end());

	for (size_t c = 0; c != size; c++) {

		if (c == 0) {
			upper.append(l_vec[c] + t + l_spaced_word + '\n');
		}
		else if (c != size - size_t(1))
		{
			upper.append(l_vec[c] + t + r_vec[c] + t + '\n');

		}
	}

	for (size_t c = 0; c != size; c++) {
		if (c == 0) {
			continue;
		}

		else if (c != size - size_t(1)) {
			lower.append(t + ' ' + l_vec[c] + t + r_vec[c] + '\n');
		}
		else {
			lower.append(r_spaced_word + t + r_vec[c] + '\n');
		}
	}

	final.append(upper);
	final.append(center);
	final.append(lower);
	return final;
}