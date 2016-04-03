#include <bits/stdc++.h>
using namespace std;

#define CATEGORY "train"

class trie {
	struct node {
		map <char, node*> children;
		double frequency;

		node(int frequency = 0) : frequency(frequency) {}
	};

	node* root;

public:
	void add_string(const string &s) {
		node* cur = root;
		cur->frequency++;
		for (int i = 0; i < s.size(); ++i) {
			if (!cur->children.count(s[i])) {
				cur->children[s[i]] = new node();
			}
			cur = cur->children[s[i]];
			cur->frequency++;
		}
	}
	void add_suffixes(const string &s) {
		for (int i = 0; i < s.size(); ++i) {
			add_string(s.substr(i));
		}
	}

	trie() : root(new node()) {}

	void go(node* v1, node* v2, node* res, double (*f)(int, int)) {
		res->frequency = f(v1->frequency, v2->frequency);
		for (auto e: v1->children) {
			if (v2->children.count(e.first)) {
				res->children[e.first] = new node();
				go(v1->children[e.first], v2->children[e.first], res->children[e.first], f);
			}
		}
	}

	trie unite(const trie &other, double (*f)(int, int)) {
		node* v1 = root;
		node* v2 = other.root;
		trie res;
		go(v1, v2, res.root, f);
		return res;
	}

	double assess_tree(node* v) {
		double res = v->frequency;
		for (auto e: v->children) {
			res += assess_tree(e.second);
		}
		return res;
	}

	double assess_tree() {
		return assess_tree(root);
	}

	void clean(node* v) {
		for (auto e: v->children) {
			clean(e.second);
		}
		delete v;
	}

	~trie() {
		clean(root);
	}
};

double assess(trie &a, trie &b, double (*f)(int, int)) {
	trie u = a.unite(b, f);
	double res = u.assess_tree();
	return res;
}

vector <string> read_docs_list() {
	ifstream in(CATEGORY"_docs_list.txt");
	vector <string> res;
	string s;
	while (in >> s) {
		res.push_back(s);
	}
	return res;
}

vector <string> format_doc(vector <string> &doc) {
	vector <string> formatted_doc;
	for (auto s: doc) {
		bool good = 1;
		for (int i = 0; i < s.size(); ++i) {
			if ('a' <= s[i] && s[i] <= 'z') {
				continue;
			} else if ('A' <= s[i] && s[i] <= 'Z') {
				//s[i] += 'a' - 'A';
				continue;
			} else {
				good = 0;
				break;
			}
		}
		if (good) {
			formatted_doc.push_back(s);
		}
	}
	return formatted_doc;
}

vector <string> read_doc(string path) {
	ifstream in("nltk_data/corpora/reuters/" + path);
	string s;
	vector <string> res;
	while (in >> s) {
		res.push_back(s);
	}
	return format_doc(res);
}

int main() {
	auto docs_list = read_docs_list();
	vector <vector <string> > docs(docs_list.size());
	for (int i = 0; i < docs_list.size(); ++i) {
		docs[i] = read_doc(docs_list[i]);
	}
	vector <trie> a(docs.size());
	for (int i = 0; i < docs.size(); ++i) {
		for (int j = 0; j < docs[i].size(); ++j) {
			a[i].add_suffixes(docs[i][j]);
		}
	}
	ofstream out(CATEGORY"_assessment_values.txt");
	out.precision(6);
	out << fixed;
	for (int i = 0; i < a.size(); ++i) {
		for (int j = 0; j < a.size(); ++j) {
			out << assess(a[i], a[j], [](int x, int y) -> double{return (x + y) / 2.0;}) << ' ';
		}
		out << '\n';
	}
	return 0;
}
