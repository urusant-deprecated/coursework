#include "trie.h"
#include <vector>

class document {
    std::string path;
	std::vector <std::string> words;
	std::string category;
	trie suffix_tree;

public:
	void format();
	void read();

	void build_suffix_tree();

	document(std::string path, std::string category);
	~document();

	double get_similarity(document &other, double (*f)(int, int));
};

std::vector <document> read_docs();
