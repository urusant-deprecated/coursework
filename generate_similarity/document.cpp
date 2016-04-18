#include "document.h"
#include "config.h"
#include <fstream>

void document::format() {
	std::vector <std::string> formatted_doc(1);
	for (auto s: words) {
		for (int i = 0; i < s.size(); ++i) {
			if ('a' <= s[i] && s[i] <= 'z') {
				formatted_doc.back() += s[i];
				continue;
			} else if ('A' <= s[i] && s[i] <= 'Z') {
				s[i] += 'a' - 'A';
				formatted_doc.back() += s[i];
				continue;
			} else {
				if (formatted_doc.back() != "") {
					formatted_doc.push_back("");
				}
				break;
			}
		}
	}
	while (formatted_doc.back().empty()) {
		formatted_doc.pop_back();
	}
	words = formatted_doc;
}

void document::read() {
	std::ifstream in(HOME_PATH + path);
	std::string s;
	while (in >> s) {
		words.push_back(s + ' ');
	}
	in.close();
	format();
	return;
}

void document::build_suffix_tree() {
	for (std::string word: words) {
		suffix_tree.add_suffixes(word);
	}
}

document::document(std::string path, std::string category) : path(path), words(0), category(category) {
	read();
	build_suffix_tree();
}

document::~document() {}

double document::get_similarity(document &other, double (*f)(int, int)) {
	return suffix_tree.assess(other.suffix_tree, f);
}

std::vector <document> read_docs() {
	std::ifstream in(HOME_PATH + LIST_PATH);
	std::string s, t;
	std::vector <document> res;
	while (in >> s >> t) {
		res.emplace_back(s, t);
	}
	in.close();
	return res;
}
