#include "trie.h"
#include "config.h"
#include <iostream>

trie::node::node(std::shared_ptr<node> parent) : parent(parent),
                                                 frequency(0) {}
trie::trie() : root_(std::make_shared<node>(std::shared_ptr<node>())) {
  //std::cerr << ++BALANCE << std::endl;
}

void trie::clean(std::shared_ptr<node> vertex) {
  while (!vertex->children.empty()) {
    clean(vertex->children.begin()->second);
    vertex->children.begin()->second.reset();
    vertex->children.erase(vertex->children.begin());
  }
}

trie::~trie() {
  //std::cerr << --BALANCE << std::endl;
  clean(root_);
}

void trie::copy(std::shared_ptr<node> destination,
                const std::shared_ptr<node> source) {
  destination->frequency = source->frequency;
  for (auto element: source->children) {
    destination->children[element.first] = std::make_shared<node>(destination);
    copy(destination->children[element.first], element.second);
  }
}

trie::trie(const trie &other) : root_(std::make_shared<node>()) {
  //std::cerr << ++BALANCE << std::endl;
  copy(root_, other.root_);
}

void trie::swap(trie& other) {std::swap(root_, other.root_);}

trie& trie::operator=(const trie &other) {
  trie tmp(other);
  swap(tmp);
  return *this;
}

void trie::add_string(const std::string &s) {
  std::shared_ptr<node> current = root_;
  current->frequency++;
  for (char c: s) {
    if (!current->children.count(c)) {
      current->children[c] = std::make_shared<node>(current);
    }
    current = current->children[c];
    current->frequency++;
  }
}

void trie::add_suffixes(const std::string &s) {
  for (int i = 0; i < s.size(); ++i) {
    add_string(s.substr(i));
  }
}

void trie::unite(std::shared_ptr<node> source1,
                 std::shared_ptr<node> source2,
                 std::shared_ptr<node> destination,
                 double (*f)(int, int)) const {
  destination->frequency = f(source1->frequency, source2->frequency);
  for (auto element: source1->children) {
    if (source2->children.count(element.first)) {
      destination->children[element.first] =
          std::make_shared<node>(destination);
      unite(source1->children[element.first], source2->children[element.first],
            destination->children[element.first], f);
    }
  }
}

double trie::assess_tree(std::shared_ptr<node> vertex) const {
  double result = 0;
  if (vertex->parent) {
    result += vertex->frequency / vertex->parent->frequency;
  }
  for (auto element: vertex->children) {
    result += assess_tree(element.second);
  }
  return result;
}

trie trie::unite(const trie &other, double (*f)(int, int)) const {
  std::shared_ptr<node> source1 = root_;
  std::shared_ptr<node> source2 = other.root_;
  trie result;
  unite(source1, source2, result.root_, f);
  return result;
}

double trie::assess_tree() const {return assess_tree(root_);}

double trie::similarity(const trie &other, double (*f)(int, int)) const {
  trie common_subtree = unite(other, f);
  return common_subtree.assess_tree();
}
