#include "document.h"
#include <algorithm>
#include <iostream>
void assessment_document::format() {
  std::vector<std::string> formatted_doc(1);
  for (auto word: words_) {
    for (char c: word) {
      if ('a' <= c && c <= 'z') {
        formatted_doc.back() += c;
      } else if ('A' <= c && c <= 'Z') {
        c += 'a' - 'A';
        formatted_doc.back() += c;
      } else {
        if (formatted_doc.back() != "") {
          formatted_doc.push_back("");
        }
        break;
      }
    }
    if (!formatted_doc.back().empty()) {
      formatted_doc.push_back("");
    }
  }
  while (formatted_doc.back().empty()) {
    formatted_doc.pop_back();
  }
  words_ = formatted_doc;
}

void assessment_document::read() {
  std::ifstream in(HOME_PATH + path_);
  std::string word;
  while (in >> word) {
    words_.push_back(word);
  }
  format();
  return;
}

document::document(const std::string &path, const std::string &category,
                   int id) : path_(path), category_(category), id_(id) {}

void assessment_document::build_suffix_tree() {
  for (std::string word: words_) {
    suffix_tree_.add_suffixes(word);
  }
}

assessment_document::assessment_document(const std::string &path,
                              const std::string &category, int id) :
                              document(path, category, id) {
  read();
  build_suffix_tree();
}

double assessment_document::get_similarity(const assessment_document &other,
                                           double (*f)(int, int)) {
  return suffix_tree_.similarity(other.suffix_tree_, f);
}

cluster_document::cluster_document(const std::string &path,
                                   const std::string &category, int id) :
                                   document(path, category, id), cluster_(-1) {}

void cluster_document::read_distances(std::vector<cluster_document> &document_list, int table) {
  sort(document_list.begin(),
       document_list.end(),
       [](cluster_document first_document, cluster_document second_document) {
         return first_document.id_ < second_document.id_;
  });
  int length = document_list.size();
  std::ifstream in(HOME_PATH + ASSESSMENT_PATH + ASSESSMENT_FILES[table]);
  for (auto &document: document_list) {
    document.distances_.resize(length);
    for (auto &e: document.distances_) {
      double similarity;
      in >> similarity;
      e = distance_by_similarity(similarity);
    }
    document.distances_[document.id_] = 0;
  }
}

std::vector<int> random_subset(int size, int range) {
  std::vector<int> result(range);
  for (int i = 0; i < range; ++i) {
    result[i] = i;
  }
  random_shuffle(result.begin(), result.end());
  while (result.size() > size) {
    result.pop_back();
  }
  return result;
}

double cluster_document::distance(int other_id) {
  return distances_[other_id];
}

void cluster_document::kmeans(int clusters, std::vector<cluster_document> &document_list) {
  int length = document_list.size();
  std::vector<int> centroid_ids = random_subset(clusters, length);
  int steps = KMEANS_STEPS;
  while (steps--) {
    //std::cerr << "Steps left: " << steps << std::endl;
    //reassigning
    for (auto &document: document_list) {
      document.cluster_ = 0;
      for (int cluster = 0; cluster < clusters; ++cluster) {
        int id = centroid_ids[cluster];
        int old_id = centroid_ids[document.cluster_];
        if (document.distance(id) < document.distance(old_id)) {
          document.cluster_ = cluster;
        }
      }
    }
    //recomputing centroids
    std::vector<double> cluster_distance(length);
    for (int i = 0; i < length; ++i) {
      for (int j = i + 1; j < length; ++j) {
        if (document_list[i].cluster_ ==
            document_list[j].cluster_) {
          cluster_distance[i] += document_list[i].distance(j);
          cluster_distance[j] += document_list[j].distance(i);
        }
      }
    }
    centroid_ids.assign(clusters, -1);
    for (auto &document: document_list) {
      if (centroid_ids[document.cluster_] == -1 ||
          cluster_distance[centroid_ids[document.cluster_]] >
          cluster_distance[document.id_]) {
        centroid_ids[document.cluster_] = document.id_;
      }
    }
  }
}

double cluster_document::compute_cost(std::vector<int> &medoids, std::vector<cluster_document> &document_list) {
  int clusters = medoids.size();
  double cost = 0;
  for (auto& document: document_list) {
    double best = 1e18;
    for (auto id: medoids) {
      best = std::min(best, document.distance(id));
    }
    cost += best;
  }
  return cost;
}

void cluster_document::recompute_clusters(std::vector<int> &medoids, std::vector<cluster_document> &document_list) {
  int clusters = medoids.size();
  for (auto &document: document_list) {
    for (int i = 0; i < clusters; ++i) {
      if (document.cluster_ == -1 ||
          document.distance(medoids[i]) <
          document.distance(medoids[document.cluster_])) {
        document.cluster_ = i;
      }
    }
  }
}

void cluster_document::kmedoids(int clusters, std::vector<cluster_document> &document_list) {
  int length = document_list.size();
  std::vector<int> medoid_ids = random_subset(clusters, length);
  recompute_clusters(medoid_ids, document_list);
  int steps = KMEDOIDS_STEPS;
  double cost = compute_cost(medoid_ids, document_list);
  while (steps-- > 0) {
    //std::cerr << steps << std::endl;
    int best_document = -1;
    double best_cost = cost;
    for (auto &document: document_list) {
      int old_medoid = medoid_ids[document.cluster_];
      if (old_medoid == document.id_) {
        continue;
      }
      medoid_ids[document.cluster_] = document.id_;
      double new_cost = compute_cost(medoid_ids, document_list);
      if (new_cost < best_cost) {
        cost = best_cost = new_cost;
        best_document = document.id_;
      }
      medoid_ids[document.cluster_] = old_medoid;
    }
    if (best_document == -1) {
      break;
    }
    auto &document = document_list[best_document];
    medoid_ids[document.cluster_] = document.id_;
    recompute_clusters(medoid_ids, document_list);
  }
}

double cluster_document::assess_clustering(int clusters, std::vector<cluster_document> &document_list) {
  int length = document_list.size();
  std::vector<std::map<std::string, int> > category_count(clusters);//for each cluster and category the number of such elements
  for (auto &document: document_list) {
    category_count[document.cluster_][document.category_]++;
  }
  int matches = 0;
  for (int cluster = 0; cluster < clusters; ++cluster) {
    int total = 0, maximum = 0;
    std::string best_category = "";
    for (auto element: category_count[cluster]) {
      total += element.second;
      if (maximum < element.second) {
        maximum = element.second;
        best_category = element.first;
      }
    }
    matches += maximum;
    std::cerr << total << ' ' << best_category << std::endl;
  }
  return (double)matches / length;
}