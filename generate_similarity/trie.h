#include <map>
#include <memory>
#include <string>

class trie {
	
	struct node {
		std::map <char, std::shared_ptr <node>> children;
		double frequency;
		std::shared_ptr <node> parent;

		node() {}
		node(std::shared_ptr <node> parent, int frequency);
	};

	std::shared_ptr <node> root;
	void add_string(const std::string &s);

	void go(std::shared_ptr <node> v1, std::shared_ptr <node> v2,
		std::shared_ptr <node> res, double (*f)(int, int));

	double assess_tree(std::shared_ptr <node> v);
public:
	trie() : root(std::make_shared <node>()) {}
	
	void add_suffixes(const std::string &s);
	
	trie unite(const trie &other, double (*f)(int, int));
	
	double assess_tree();

	double assess(trie &b, double (*f)(int, int));
};
