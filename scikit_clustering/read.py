import csv

def read_similarity(filename):
    file = open(filename, "r")
    matrix = []
    for row in csv.reader(file, delimiter=' '):
        matrix.append(map(int, raw))
    return matrix