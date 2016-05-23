from sklearn import cluster
import csv

def read_similarity(filename):
    file = open(filename, 'r')
    matrix = []
    for row in csv.reader(file, delimiter=' ', lineterminator=' \n'):
        row.pop()
        matrix.append(row)
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            matrix[i][j] = float(matrix[i][j]);
    return matrix

def read_categories():
    f = open('/home/urusant/Coursework/docs_list.txt', 'r')
    result = []
    a = dict()
    for line in f:
        result.append(line.split())
        a[result[-1][1]] = 0;
    for x in result:
        a[x[1]] += 1
 #   for e in a:
#        print(e, a[e])
    return result

def custom_spectral_cluster(similarity, cluster_count):
    clustering = cluster.SpectralClustering(cluster_count, affinity='precomputed', eigen_solver='arpack')
    clustering.fit(similarity)
    clusters = clustering.fit_predict(similarity)
    return list(clusters)

def custom_agglomerative_cluster(similarity, cluster_count):
    clustering = cluster.AgglomerativeClustering(cluster_count, connectivity=similarity)
    clustering.fit(similarity)
    clusters = clustering.fit_predict(similarity)
    return list(clusters)

similarity = read_similarity('/home/urusant/Coursework/assessment/0.csv')
documents = read_categories()
clusters = custom_spectral_cluster(similarity, 8)
category_count = []
for i in range(8):
    category_count.append(dict())
n = len(documents)
for i in range(n):
    if documents[i][1] not in category_count[clusters[i]]:
        category_count[clusters[i]][documents[i][1]] = 0
    category_count[clusters[i]][documents[i][1]] += 1
matches = 0
for c in range(8):
    total = 0
    maximum = 0
    best_category = ''
    for e in category_count[c]:
        total += category_count[c][e]
        if maximum < category_count[c][e]:
            maximum = category_count[c][e]
            best_category = e
    matches += maximum
    print(total, best_category)
print(matches / n)