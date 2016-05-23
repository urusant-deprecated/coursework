#include "trie.h"
#include "config.h"
#include <fstream>
#include <vector>

class document {
 public:
  document(const std::string &path, const std::string &category, int id);
 protected:
  std::string path_;
  std::string category_;
  int id_;
};

class assessment_document : public document {
 public:
  assessment_document(const std::string &path,
                      const std::string &category, int id);
  double get_similarity(const assessment_document &other,
                        double (*f)(int, int));
 protected:
  trie suffix_tree_;
  std::vector<std::string> words_;
  void build_suffix_tree();  
  void format();
  void read();
};

class cluster_document : public document {
 public:
  cluster_document(const std::string &path,
                   const std::string &category, int id);
  double distance(int other_id);
  static void read_distances(std::vector<cluster_document> &document_list, int table);
  static void kmeans(int clusters, std::vector<cluster_document> &document_list);
  static void kmedoids(int clusters, std::vector<cluster_document> &document_list);
  static double assess_clustering(int clusters, std::vector<cluster_document> &document_list);
 protected:
  int cluster_;
  std::vector<double> distances_;
  static double compute_cost(std::vector<int> &medoids, std::vector<cluster_document> &document_list);
  static void recompute_clusters(std::vector<int> &medoids, std::vector<cluster_document> &document_list);
};

template <class document_type>
std::vector <document_type> read_docs() {
  std::ifstream in(HOME_PATH + LIST_PATH);
  std::string path, category;
  std::vector <document_type> document_list;
  int current_id = 0;
  while (in >> path >> category) {
    document_list.emplace_back(path, category, current_id++);
  }
  in.close();
  return document_list;
}