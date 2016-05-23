#include <map>
#include <memory>
#include <string>

//This class stores suffixes of all
//words that were added to it as a trie,
//annotating them with frequencies
class trie {
 public:
  trie();
  ~trie();
  trie(const trie&);
  trie& operator=(const trie &other);
  void swap(trie&);

  void add_suffixes(const std::string&);
  trie unite(const trie &other, double (*f)(int, int)) const;
  double assess_tree() const;
  double similarity(const trie &other, double (*f)(int, int)) const;

 private:
  struct node {
    std::map <char, std::shared_ptr<node>> children;
    double frequency;
    std::shared_ptr<node> parent;

    node() {}
    node(std::shared_ptr<node> parent);
  };
  std::shared_ptr<node> root_;
  void add_string(const std::string&);
  void unite(std::shared_ptr<node> vertex1, std::shared_ptr<node> vertex2,
             std::shared_ptr<node> result, double (*f)(int, int)) const;
  double assess_tree(std::shared_ptr<node> vertex) const;
  void copy(std::shared_ptr<node> destination, const std::shared_ptr<node> source);
  void clean(std::shared_ptr<node> vertex);
};
