from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity
from string import punctuation

def read_text(filename):
	f = open("/home/urusant/Coursework/" + filename)
	s = f.read()
	s = ' '.join(s.split())
	return ''.join(ch for ch in s if ch.isalpha() or ch.isspace())

def read_texts():
	f = open("/home/urusant/Coursework/docs_list.txt")
	a = []
	for line in f:
		a.append((read_text(line.split()[0]), line.split()[1]))
	return a

a = read_texts()
vectorizer = TfidfVectorizer()
matrix = vectorizer.fit_transform([x[0] for x in a])
cosine_sim2 = cosine_similarity(matrix, matrix)

def custom_spectral_cluster(similarity, cluster_count):
    clustering = cluster.SpectralClustering(cluster_count, affinity='precomputed', eigen_solver='arpack')
    clustering.fit(similarity)
    clusters = clustering.fit_predict(similarity)
    return list(clusters)

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
#    for e in a:
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

def write_similarity(filename, similarity):
	f = open(filename, 'w')
	for line in similarity:
		for element in line:
			f.write(str(element) + ' ')
		f.write('\n')
	return

similarity = cosine_sim2 #read_similarity('/home/urusant/Coursework/assessment/1.csv')
#write_similarity("/home/urusant/Coursework/assessment/0.csv", similarity)
documents = read_categories()
clusters = custom_spectral_cluster(similarity, 10)
category_count = []
for i in range(10):
    category_count.append(dict())
n = len(documents)
for i in range(n):
    if documents[i][1] not in category_count[clusters[i]]:
        category_count[clusters[i]][documents[i][1]] = 0
    category_count[clusters[i]][documents[i][1]] += 1
matches = 0
for c in range(10):
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