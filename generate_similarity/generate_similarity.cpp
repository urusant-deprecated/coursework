#include "document.h"
#include "config.h"
#include <iostream>
#include <fstream>


int main() {
  auto docs = read_docs();
  std::vector<std::vector<double>> similarity[TABLES];
  for (int table = 0; table < TABLES; ++table) {
    similarity[table].resize(docs.size(), std::vector<double>(docs.size(), 0));
  }
  for (int i = 0; i < docs.size(); ++i) {
    std::cerr << i << std::endl;
    for (int j = i + 1; j < docs.size(); ++j) {
      for (int table = 0; table < TABLES; ++table) {
        similarity[table][i][j] = similarity[table][j][i] =
            docs[i].get_similarity(docs[j], functions[table]);
      }
    }
  }
  for (int table = 0; table < TABLES; ++table) {
    std::ofstream out(HOME_PATH + ASSESSMENT_PATH + ASSESSMENT_FILES[table]);
    out.precision(2);
    out << std::fixed;
    for (int i = 0; i < docs.size(); ++i) {
      for (int j = 0; j < docs.size(); ++j) {
        out << similarity[table][i][j] << ' ';
      }
      out << std::endl;
    }
    out.close();
  }
  return 0;
}
