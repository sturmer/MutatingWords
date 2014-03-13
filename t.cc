#include "graph.hpp"
#include "util.hpp"
#include <cpptest.h>
#include <iostream>
#include <string>

using std::endl;
using std::cout;
using std::cerr;
using std::string;

int main(int argc, const char* argv[])
{

  Test::TextOutput output(Test::TextOutput::Verbose);
  UtilTestSuite gts;
  gts.run(output);

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " w1 w2\n";
        return -1;
    }
    string word1 = argv[1];
    string word2 = argv[2];
    
    if (word1.size() != word2.size()) {
        return -2;
    }

    buildList("words.txt", word1.size());
    //    printHistogram("words.txt");
    Graph<string> g("filtered.txt");

    //g.Print("parsed.txt");
    cout << "Graph with " << g.GetNumNodes() << " nodes and " << 
      g.GetNumEdges() << " edges\n";
    g.Dijkstra(word1);
    cout << "Dist(" << word1 << ", " << word2 << "): " << 
      g.GetDistance(word1, word2) << endl;

    string p(g.parent[word2]);
    cout << word2 << " <- ";
    while (p != "nil") {
      cout << p << " <- ";
      p = g.parent[p];
    }
    cout << word1 << '\n';

    return 0;
}

