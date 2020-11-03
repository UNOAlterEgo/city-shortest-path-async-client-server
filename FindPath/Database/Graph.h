#pragma once
#include "../Database/UML/City.h"
#include "../Database/DatabaseManager.h"
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <string>
#include <unordered_map>
#include <fstream>
#include <mutex>          // std::mutex, std::unique_lock
#include <shared_mutex>
#include <set>
#include <vector>
#include <utility>      // std::pair
#include <limits.h> 
#include <stack>

using namespace std;

// A class to represent a graph. A graph 
// is an map of adjacency lists. 
// Size of map will be V (number of vertices 
// in graph) 
class Graph
{
public:
	// Constructor optional file name
	Graph(DatabaseManager& data, string graphFileName = "");
	// Init graph
	bool Graph::initGraph();
	// To add an edge 
	bool addEdge(int idU, int idV, bool saveOnFile = true);
	// To add an edge 
	bool updateGraph(City& u, City& v);
	// Prints shortest paths from src to all other vertexes
	string  getShortestPath(int src, int dest);
	// Saves graph
	bool save();
	// Prints Graph
	friend ostream& operator<<(ostream& os, const Graph& city);

private:
	//  Adjacency list
	// <city id, <cityB id, cityB wieght>>
	unordered_map<int, set<int>> adj;

	// Value for INF
	const int INF = INT_MAX;
	mutable ::shared_mutex mutexGraph_;
	DatabaseManager& database;
	// DB file name
	string graphFileName = "graph.txt";

};
