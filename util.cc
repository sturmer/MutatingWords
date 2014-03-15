#include "util.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unordered_map>

using std::cout;
using std::ifstream;
using std::ofstream;
using std::unordered_map;
using std::endl;


void buildList(const string& filename, const size_t i)
{
    ifstream f(filename);
    if (!f) {
        throw std::exception();
    }

    ofstream out("filtered.txt");
    if (!out) {
        throw std::exception();
    }

    static const int MAX_WORD_LENGTH = 128;
    char line[MAX_WORD_LENGTH];
    while (f.getline(line, sizeof(line))) {
        string sline(line);
        if (sline.size() == i) {
            out << sline << '\n';
        }
    }
}

typedef size_t StringLengthT;
typedef int NumSamplesT;
void printHistogram(const string& filename)
{
    unordered_map<StringLengthT, NumSamplesT> hist;
    // Scroll the list of words to print a histogram. With '#'. TODO 
    ifstream f(filename);
    if (!f) {
        throw std::exception();
    }
    char line[256];
    while (f.getline(line, sizeof(line))) {
      string sline(line);
      if (hist.find(sline.size()) != hist.end()) {
	hist[sline.size()] += 1;
      } else {
	hist[sline.size()] = 1;
      }
    }
    for (auto col: hist) {
      cout << col.first << ": " << col.second << endl;
    }
}

int getHammingDistance(const string& s1, const string& s2)
{
    if (s1.size() != s2.size()) {
        return Infinity;
    }
    int distance = 0;
    for (int i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i]) {
            ++distance;
        }
    }
    return distance;
}

void UtilTestSuite::test_get_hamming_distance()
{
  TEST_ASSERT(getHammingDistance("wind", "wand") == 1);
  TEST_ASSERT(getHammingDistance("wind", "wane") == 2);
  TEST_ASSERT(getHammingDistance("wind", "list") == 3);
  TEST_ASSERT(getHammingDistance("wind", "crow") == 4);
  TEST_ASSERT(getHammingDistance("wind", "winds") == Infinity);
}
