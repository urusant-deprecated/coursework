#include "document.h"
#include "config.h"
#include <iostream>
#include <fstream>

int main() {
	auto docs = read_docs();
	std::vector <std::vector <double> > similarity[TABLES];
	for (int i = 0; i < TABLES; ++i) {
		similarity[i].resize(docs.size(), std::vector <double>(docs.size(), 0));
	}
	for (int i = 0; i < docs.size(); ++i) {
		std::cerr << i << std::endl;
		for (int j = i + 1; j < docs.size(); ++j) {
			for (int f = 0; f < TABLES; ++f) {
				similarity[f][i][j] = similarity[f][j][i] =
					docs[i].get_similarity(docs[j], functions[f]);
			}
		}
	}
	for (int f = 0; f < TABLES; ++f) {
		std::ofstream out(HOME_PATH + ASSESSMENT_PATH + ASSESSMENT_FILES[f]);
		out.precision(2);
		out << std::fixed;
		for (int i = 0; i < docs.size(); ++i) {
			for (int j = 0; j < docs.size(); ++j) {
				out << similarity[f][i][j] << ' ';
			}
			out << std::endl;
		}
		out.close();
	}
	return 0;
}
