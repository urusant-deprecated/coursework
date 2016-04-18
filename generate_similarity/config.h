#include <cmath>
#include <string>

const std::string HOME_PATH = "/home/anton/Coursework/";
const std::string ASSESSMENT_PATH = "assessment/";
const std::string LIST_PATH = "docs_list.txt";
const int PRECISION = 2;
const int TABLES = 4;

const std::string ASSESSMENT_FILES[TABLES] = {
  "1.csv",
  "2.csv",
  "3.csv",
  "4.csv"
};

typedef double (*assessment_function)(int, int);

const assessment_function functions[TABLES] = {
  [](int x, int y) -> double{return (x + y) / 2.0;},
  [](int x, int y) -> double{return std::min(x, y);},
  [](int x, int y) -> double{return std::max(x, y);},
  [](int x, int y) -> double{return std::sqrt(x * y * 1.);}
};