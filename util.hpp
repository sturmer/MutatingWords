#ifndef UTIL_HPP
#define UTIL_HPP

#include <cpptest.h>

#include <string>
#include <limits>

using std::string;
using std::numeric_limits;

static const int Infinity = {10000000};

/* Only include in the list the words of length `i' */
void buildList(const string& filename, const size_t i);

void printHistogram(const string& filename);

int getHummingDistance(const string& s1, const string& s2);

class UtilTestSuite : public Test::Suite
{
public:
  UtilTestSuite()
  {
    TEST_ADD(UtilTestSuite::test_get_humming_distance);
  }
private:
  void test_get_humming_distance();
};

#endif
