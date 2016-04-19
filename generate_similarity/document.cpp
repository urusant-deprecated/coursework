#include "document.h"
#include "config.h"
#include <fstream>

//for debug
#include <iostream>
void document::format() {
  std::vector<std::string> formatted_doc(1);
  for (auto word: words_) {
    for (char c: word) {
      if ('a' <= c && c <= 'z') {
        formatted_doc.back() += c;
      } else if ('A' <= c && c <= 'Z') {
        c += 'a' - 'A';
        formatted_doc.back() += c;
      } else {
        if (formatted_doc.back() != "") {
          formatted_doc.push_back("");
        }
        break;
      }
    }
    if (!formatted_doc.back().empty()) {
      formatted_doc.push_back("");
    }
  }
  while (formatted_doc.back().empty()) {
    formatted_doc.pop_back();
  }
  words_ = formatted_doc;
}

void document::read() {
  std::ifstream in(HOME_PATH + path_);
  std::string word;
  while (in >> word) {
    words_.push_back(word);
  }
  format();
  return;
}

void document::build_suffix_tree() {
  for (std::string word: words_) {
    suffix_tree_.add_suffixes(word);
  }
}

document::document(const std::string &path, const std::string &category) :
                   path_(path), words_(0), category_(category) {
  read();
  build_suffix_tree();
}

double document::get_similarity(document &other, double (*f)(int, int)) {
  return suffix_tree_.similarity(other.suffix_tree_, f);
}

std::vector <document> read_docs() {
  std::ifstream in(HOME_PATH + LIST_PATH);
  std::string path, category;
  std::vector <document> document_list;
  while (in >> path >> category) {
    document_list.emplace_back(path, category);
  }
  in.close();
  return document_list;
}
