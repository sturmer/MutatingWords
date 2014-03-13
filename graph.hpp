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
};

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
    if (getHummingDistance(node.first, s) == 1) {
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
  SortByDistance(const Graph<T>& g) : g_(g) {}
  bool operator()(const T& t1, const T& t2)
  {
    return g_.dist[t1] < g_.dist[t2];
  }
      
private:
  Graph<T> g_;
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
    SortByDistance<T> sbd(*this);
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

