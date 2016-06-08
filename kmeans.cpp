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

void kmeans(int k, vector <int> &cluster, vector <vector <double> > &distance) {
  int n = cluster.size();
  int ITER = 60;
  vector <int> center = random(k, n);
  while (ITER--) {
    //reassigning
    for (int i = 0; i < n; ++i) {
      cluster[i] = 0;
      for (int j = 0; j < k; ++j) {
        if (distance[center[j]][i] < distance[center[cluster[i]]][i]) {
          cluster[i] = j;
        }
      }
    }
    //recomputing centroids
    vector <double> dist(n);
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        if (cluster[i] == cluster[j]) {
          dist[i] += distance[i][j] * distance[i][j];
          dist[j] += distance[i][j] * distance[i][j];
        }
      }
    }
    center.assign(k, -1);
    for (int i = 0; i < n; ++i) {
      if (center[cluster[i]] == -1 ||
        dist[center[cluster[i]]] > dist[i]) {
        center[cluster[i]] = i;
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
  auto docs = read_docs<cluster_document>();
  ifstream in("assessment_values1.csv");
  vector <vector <double> > distance(docs_list.size(), vector <double>(docs_list.size(), 0));
  for (int i = 0; i < docs_list.size(); ++i) {
    for (int j = 0; j < docs_list.size(); ++j) {
      double x;
      in >> x;
      if (i != j)
        distance[i][j] = 1000.0 / x;
    }
  }
  srand(time(NULL));
  vector <int> cluster(docs_list.size());
  int k = 10;
  kmeans(10, cluster, distance);
  vector <map <string, int> > b(k);
  for (int i = 0; i < docs_list.size(); ++i) {
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
  for (int i = 0; i < docs_list.size(); ++i) {
    if (docs_list[i].second == classes[cluster[i]]) {
      cnt_good++;
    }
  }
  cout << (double)cnt_good / (double)docs_list.size() << endl;
  return 0;
}
