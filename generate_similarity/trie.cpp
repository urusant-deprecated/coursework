#include "trie.h"

trie::node::node(std::shared_ptr <node> parent, int frequency = 0) : parent(parent), frequency(frequency) {}

void trie::add_string(const std::string &s) {
	std::shared_ptr <node> cur = root;
	cur->frequency++;
	for (int i = 0; i < s.size(); ++i) {
		if (!cur->children.count(s[i])) {
			cur->children[s[i]] = std::make_shared <node>(cur);
		}
		cur = cur->children[s[i]];
		cur->frequency++;
	}
}

void trie::go(std::shared_ptr <node> v1, std::shared_ptr <node> v2, std::shared_ptr <node> res, double (*f)(int, int)) {
	res->frequency = f(v1->frequency, v2->frequency);
	for (auto e: v1->children) {
		if (v2->children.count(e.first)) {
			res->children[e.first] = std::make_shared <node>(res);
			go(v1->children[e.first], v2->children[e.first], res->children[e.first], f);
		}
	}
}

double trie::assess_tree(std::shared_ptr <node> v) {
	double res = 0;
	if (v->parent) {
		res += v->frequency / v->parent->frequency;
	}
	for (auto e: v->children) {
		res += assess_tree(e.second);
	}
	return res;
}

void trie::add_suffixes(const std::string &s) {
	for (int i = 0; i < s.size(); ++i) {		
		add_string(s.substr(i));
	}
}

trie trie::unite(const trie &other, double (*f)(int, int)) {
	std::shared_ptr <node> v1 = root;
	std::shared_ptr <node> v2 = other.root;
	trie res;
	go(v1, v2, res.root, f);
	return res;
}


double trie::assess_tree() {
	return assess_tree(root);
}

double trie::assess(trie &b, double (*f)(int, int)) {
	trie u = unite(b, f);
	double res = u.assess_tree();
	return res;
}
