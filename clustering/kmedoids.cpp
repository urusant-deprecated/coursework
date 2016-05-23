#include <bits/stdc++.h>
using namespace std;

vector <int> random(int k, int n) {
	vector <int> a(n);
	for (int i = 0; i < n; ++i) {
		a[i] = i;
	}
	random_shuffle(a.begin(), a.end());
	while (a.size() > k) {
		a.pop_back();
	}
	return a;
}

double compute_cost(vector <int> &medoids, vector <vector <double> > &distance) {
	int n = distance.size();
	int k = medoids.size();
	double cost = 0;
	for (int i = 0; i < n; ++i) {
		double cur = distance[i][medoids[0]];
		for (int j = 1; j < k; ++j) {
			cur = min(cur, distance[i][medoids[j]]);
		}
		cost += cur;
	}
	return cost;
}

void kmedoids(int k, vector <int> &cluster, vector <vector <double> > &distance) {
	int n = distance.size();
	vector <int> medoids = random(n, k);
	double cost = compute_cost(medoids, distance);
	bool changed = 1;
	int ITER = 100;
	while (ITER--) {
		cerr << "OK" << endl;
		changed = 0;
		for (int &m: medoids) {
			if (changed) {
				break;
			}
			for (int o = 0; o < n; ++o) {
				if (medoids[cluster[o]] == o) {//o is a medoid
					continue;
				}
				swap(m, o);
				double cost2 = compute_cost(medoids, distance);
				if (cost2 < cost) {
					changed = 1;
					cost = cost2;
				} else {
					swap(m, o);
				}
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		cluster[i] = 0;
		for (int j = 1; j < k; ++j) {
			if (distance[medoids[j]][i] < distance[medoids[cluster[i]]][i]) {
				cluster[i] = j;
			}
		}
	}
	return;
}

vector <pair <string, string> > read_docs_list() {
	ifstream in("docs_list.txt");
	vector <pair <string, string> > res;
	string s, t;
	while (in >> s >> t) {
		res.push_back({s, t});
	}
	return res;
}

int main() {
	auto docs_list = read_docs_list();
	vector <vector <string> > docs(docs_list.size());
	ifstream in("assessment_values1.csv");
	vector <vector <double> > distance(docs.size(), vector <double>(docs.size(), 0));
	for (int i = 0; i < docs.size(); ++i) {
		for (int j = 0; j < docs.size(); ++j) {
			double x;
			in >> x;
			if (i != j)
				distance[i][j] = 1000.0 / x;
		}
	}
	srand(time(NULL));
	vector <int> cluster(docs.size());
	int k = 10;
	kmedoids(10, cluster, distance);
	vector <map <string, int> > b(k);
	for (int i = 0; i < docs.size(); ++i) {
		b[cluster[i]][docs_list[i].second]++;
	}
	vector <string> classes(k);
	for (int i = 0; i < k; ++i) {
		vector <string> best;
		int value = 0;
		int cluster_size = 0;
		for (auto e: b[i]) {
			cluster_size += e.second;
			if (e.second > value) {
				best.clear();
				best.push_back(e.first);
				value = e.second;
			} else if (e.second == value) {
				best.push_back(e.first);
			}
		}
		cerr << cluster_size << endl;
		classes[i] = best[rand() % best.size()];
	}
	int cnt_good = 0;
	for (int i = 0; i < docs.size(); ++i) {
		if (docs_list[i].second == classes[cluster[i]]) {
			cnt_good++;
		}
	}
	cout << (double)cnt_good / (double)docs.size() << endl;
	return 0;
}
