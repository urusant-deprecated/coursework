#include "document.h"
#include <iostream>
using namespace std;

int main() {
	auto document_list = read_docs<cluster_document>();
	cluster_document::read_distances(document_list, CLUSTERING_TABLE);
	std::cout << "Table " << CLUSTERING_TABLE << std::endl;
	cluster_document::kmedoids(CLUSTERS, document_list);
	std::cout << cluster_document::assess_clustering(CLUSTERS, document_list) << std::endl;
}