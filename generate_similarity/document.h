#include "trie.h"
#include <vector>

class document {
 public:
  document(const std::string &path, const std::string &category);
  ~document() {}
  double get_similarity(document &other, double (*f)(int, int));
 private:
  std::string path_;
  std::vector<std::string> words_;
  std::string category_;
  trie suffix_tree_;

  void format();
  void read();
  void build_suffix_tree();
};

std::vector<document> read_docs();