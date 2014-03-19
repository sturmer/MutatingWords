#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "prettyprint.hpp"
#include "util.hpp"

#include <cpptest.h>

#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>

using std::priority_queue;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::cout;
using std::cerr;
using std::pair;
using std::queue;
using std::iterator;

template <typename T>
T parseLine(const string& line);

template <>
string parseLine<string>(const string& line)
{
    return line;
}

class GraphTestSuite;

template <typename T>
class Graph {
  unordered_map<T, vector<T>> adjlist;
  vector<T> nodes;
  unordered_map<T, bool> discovered;
  int num_edges;
  
public:
  Graph(const string& filename);
  void AddNode(const T& n);
  void Dijkstra(const T& start);
  //  void DoBfs(const T& start, std::function<vector<T>>());
  int GetDistance(const T& n, const T& m);
  void Print(const string& out_file) const;
  int GetNumNodes() const;
  int GetNumEdges() const;

  unordered_map<T, int> dist;
  unordered_map<T, T> parent;
  friend class GraphTestSuite;
};

class GraphTestSuite : public Test::Suite
{
public: GraphTestSuite()
  {
    TEST_ADD(GraphTestSuite::test_AddNode);
    TEST_ADD(GraphTestSuite::test_Dijkstra);
  }
protected:
  virtual void setup();
  virtual void tear_down();
private:
  void test_AddNode();
  void test_Dijkstra();
  Graph<string>* g;
};

void GraphTestSuite::setup()
{
  g = new Graph<string>("t/test2.txt");
}

void GraphTestSuite::tear_down()
{
  delete g;
}

void GraphTestSuite::test_AddNode()
{
  TEST_ASSERT(g->adjlist["typo"].size() == 1 && g->adjlist["typo"][0] == "type");
  TEST_ASSERT(g->adjlist["type"].size() == 2);
  TEST_ASSERT(g->adjlist["tape"].size() == 2);
  TEST_ASSERT(g->adjlist["tale"].size() == 2);
  TEST_ASSERT(g->adjlist["male"].size() == 2);
  TEST_ASSERT(g->adjlist["mile"].size() == 4);
  TEST_ASSERT(g->adjlist["milk"].size() == 2);
  TEST_ASSERT(g->adjlist["silk"].size() == 2);
  TEST_ASSERT(g->adjlist["sick"].size() == 3);
  TEST_ASSERT(g->adjlist["tick"].size() == 2);
  TEST_ASSERT(g->adjlist["lick"].size() == 3);
  TEST_ASSERT(g->adjlist["link"].size() == 2);
  TEST_ASSERT(g->adjlist["line"].size() == 2);
  TEST_ASSERT(g->adjlist["mine"].size() == 3);
  TEST_ASSERT(g->adjlist["mice"].size() == 3);
  TEST_ASSERT(g->adjlist["rice"].size() == 1 && g->adjlist["rice"][0] == "mice");

  string s1("file");
  g->AddNode(s1);
  TEST_ASSERT(g->adjlist["mile"].size() == 5);
  TEST_ASSERT(std::find(g->adjlist["mile"].begin(),
			g->adjlist["mile"].end(), s1) !=
	      g->adjlist["mile"].end());
  TEST_ASSERT(g->adjlist["file"].size() == 1 && g->adjlist["file"][0] == "mile");
  string s2("five");
  g->AddNode(s2);
  TEST_ASSERT(std::find(g->adjlist["mile"].begin(),
			g->adjlist["mile"].end(), s2) ==
	      g->adjlist["mile"].end());
  TEST_ASSERT(g->adjlist["five"].size() == 1 && g->adjlist["five"][0] == "file");
  TEST_ASSERT(g->adjlist["file"].size() == 2);
}

void GraphTestSuite::test_Dijkstra()
{
  // "five" and "file" are not in the graph, so setup is called
  // again at the beginning of this function?
  TEST_ASSERT(g->GetNumNodes() == 16);
  // g->Dijkstra("rice");  // rice is in the file
  // TEST_ASSERT(g->dist["typo"] == 7);
  // TEST_ASSERT(std::find(g->adjlist["rice"].begin(),
  // 			g->adjlist["rice"].end(),
  // 			"typo") == g->adjlist["rice"].end());
  // TEST_ASSERT(std::find(g->adjlist["rice"].begin(),
  // 			g->adjlist["rice"].end(),
  // 			"mice") != g->adjlist["rice"].end());
  g->Dijkstra("typo");
  TEST_ASSERT(g->dist["rice"] == 7);
  TEST_ASSERT(std::find(g->adjlist["typo"].begin(),
			g->adjlist["typo"].end(),
			"rice") == g->adjlist["typo"].end());
  TEST_ASSERT(std::find(g->adjlist["typo"].begin(),
			g->adjlist["typo"].end(),
			"type") != g->adjlist["typo"].end());
}

template <typename T>
int Graph<T>::GetDistance(const T& a, const T& b)
{
  return dist[b];
}

template <typename T>
Graph<T>::Graph(const string& filename) : num_edges(0)
{
    ifstream f(filename.c_str());
    if (!f) {
        throw std::exception();
    }

    static const int MAX_WORD_LENGTH = 128;
    char line[MAX_WORD_LENGTH];
    while (f.getline(line, sizeof(line))) {
      T lineParsed = parseLine<T>(line);
        AddNode(lineParsed);
    }
}

template <typename T>
void Graph<T>::AddNode(const T& s)
{
  nodes.push_back(s);
  if (adjlist.find(s) == adjlist.end()) {
    std::vector<T> emptyvector;
    adjlist[s] = emptyvector;
  }
  
  for (auto node: adjlist) {
    if (getHammingDistance(node.first, s) == 1) {
      adjlist[node.first].push_back(s);
      adjlist[s].push_back(node.first);
      ++num_edges;
    } 

  }
}

//template <typename T>
//void Graph<T>::DoBfs(const T& start, std::function<vector<T>>())
//{
//    queue<T> q;
//    q.push(start)
//}

template <typename T>
void Graph<T>::Print(const string& out_file) const 
{
  ofstream f(out_file);
  if (!f) {
    throw std::exception();
  }

    for (auto word: adjlist) {
        f << word.first << ": ";
        for (auto onedist : word.second) {
            f << onedist << " ";
        }
        f << '\n';
    }
}

template <typename T>
class SortByDistance {
public:
  SortByDistance(Graph<T>* g) : g_(g) {}
  bool operator()(const T& t1, const T& t2)
  {
    return g_->dist[t1] < g_->dist[t2];
  }
      
private:
  Graph<T>* g_;
};

template <typename T>
void Graph<T>::Dijkstra(const T& start)
{
    for (auto node : nodes) {
        dist[node] = Infinity;
	parent[node] = "nil";
    }
    bool no_neighbors = true;
    for (auto neighbor : adjlist[start]) {
        dist[neighbor] = 1;
        no_neighbors = false;
    }
    dist[start] = 0;
    if (no_neighbors) {
        cerr << start << " is an isolated node!\n";
        return;
    }

    vector<T> q = nodes;
    SortByDistance<T> sbd(this);
    while (!q.empty()) {
      typename vector<T>::iterator vptr =
	min_element(q.begin(), q.end(), sbd);
      q.erase(vptr);
      T v = *vptr;      // check pointer?
      for (auto edge: adjlist[v]) {
	if (dist[v] + 1 < dist[edge]) {
	  dist[edge] = dist[v] + 1;
	  parent[edge] = v;
	  cout << "Reach " << edge << " through " << v << " in " <<
	    (dist[v]+1) << " steps\n";
	}
      }
    }
}

template <typename T>
int Graph<T>::GetNumNodes() const
{
    return nodes.size();
}

template <typename T>
int Graph<T>::GetNumEdges() const
{
    return num_edges;
}

#endif // GRAPH_HPP

