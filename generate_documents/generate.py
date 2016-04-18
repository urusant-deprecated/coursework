from reuters_parser import stream_reuters_documents
from config import good_categories, PATH, LIST_PATH, REUTERS_PATH, DOCUMENTS_PATH
good_documents = open(PATH + LIST_PATH, 'w')

for document in stream_reuters_documents(PATH + REUTERS_PATH):
    if document['body'] == "" or document['title'] == "":
        continue
    count = 0
    found_category = ""
    for category in good_categories:
        if category in document['topics']:
            count += 1
            found_category = category
    if count != 1:
        continue
    good_documents.write(DOCUMENTS_PATH + str(document['id']) + ' ' + found_category + '\n')
    document_file = open(PATH + DOCUMENTS_PATH + str(document['id']), 'w')
    document_file.write(document['title'] + '\n' + document['body'])
