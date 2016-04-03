from nltk.corpus import reuters

train_docs = list(filter(lambda doc: doc.startswith("train"), reuters.fileids()))
test_docs = list(filter(lambda doc: doc.startswith("test"), reuters.fileids()))

def my_filter(docs):
	good_categories = set(["acq", "corn", "crude", "earn", "grain", "interest", "money-fx", "ship", "trade", "wheat"])
	good_documents = []
	for d in docs:
		count = 0
		for c in good_categories:
			if d in reuters.fileids(c):
				count += 1
		if count == 1:
			good_documents.append(d)
	return good_documents

train_docs = my_filter(train_docs)
test_docs = my_filter(test_docs)
print(len(train_docs))
print(len(test_docs))
f = open("train_docs_list.txt", "w")
for d in train_docs:
	f.write(d + '\n')
f = open("test_docs_list.txt", "w")
for d in test_docs:
	f.write(d + '\n')
