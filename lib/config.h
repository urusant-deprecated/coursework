#include <cmath>
#include <string>

const std::string HOME_PATH = "/home/urusant/Coursework/";
const std::string ASSESSMENT_PATH = "assessment/";
const std::string LIST_PATH = "docs_list.txt";
const int PRECISION = 2;
const int TABLES = 4;

const std::string ASSESSMENT_FILES[TABLES + 1] = {
  "1.csv",
  "2.csv",
  "3.csv",
  "4.csv",
  "0.csv"
};

typedef double (*assessment_function)(int, int);

const assessment_function functions[TABLES] = {
  [](int x, int y) -> double{return (x + y) / 2.0;},
  [](int x, int y) -> double{return std::min(x, y);},
  [](int x, int y) -> double{return std::max(x, y);},
  [](int x, int y) -> double{return std::sqrt(x * y * 1.);}
};

double distance_by_similarity(double similarity);

const int KMEANS_STEPS = 60;
const int KMEDOIDS_STEPS = 60;
const int CLUSTERS = 8;
const int CLUSTERING_TABLE = 4;