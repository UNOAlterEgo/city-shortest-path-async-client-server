#include "Graph.h"

using namespace std;

Graph::Graph(DatabaseManager& database, string graphFileName) : database(database)
{
	if (graphFileName != "")
	{
		this->graphFileName = graphFileName;
	}

	initGraph();
}

bool Graph::initGraph() {
	shared_lock<shared_mutex> lockGraph(mutexGraph_);

	ifstream infile(graphFileName);

	// File doesn't exist
	if (infile.fail())
	{
		cout << "File " << graphFileName << " does not exist." << endl;
		// create file if it doesn't exist
		ofstream outfile(graphFileName);
		if (outfile.fail())
			return false;
		outfile.close();
	}

	cout << "Initializing Graph..." << endl;
	int idU, idV;
	while (infile >> idU >> idV)
	{
		lockGraph.unlock();
		// successfully extracted one line
		addEdge(idU, idV, false);
		lockGraph.lock();
	}

	infile.close();
	cout << "Done." << endl << endl;

	return true;
}

// To add an edge 
bool Graph::addEdge(int idU, int idV, bool saveOnFile)
{
	if (idU == idV)
	{
		cout << "Can not create an edge between equal cities" << endl;
		return false;
	}

	const map<int, City>& cities = database.getCities();
	unique_lock<shared_mutex> lockGraph(mutexGraph_);

	// tacking lock on DB data structure
	auto iteratorU = cities.find(idU);
	auto iteratorV = cities.find(idV);
	if (iteratorU != cities.end() && iteratorV != cities.end())
	{
		// found
		City cityU = iteratorU->second;
		City cityV = iteratorV->second;
		cout << "Edge <" << cityU.id << ", " << cityV.id << "> inserted in Graph" << endl;
		adj[idU].insert(idV);
		adj[idV].insert(idU);

		if (saveOnFile)
			return save();
		else
			return true;
	}
	else
	{
		if (iteratorU == cities.end())
		{
			// not found u
			cout << "ERROR - Not possible to add city with id" << idU << ": not found in DB" << endl;
		}
		if (iteratorV == cities.end())
		{
			// not found v
			cout << "ERROR - Not possible to add city with id" << idV << ": not found in DB" << endl;
		}
	}

	return false;
}

// To update values in the graph 
bool Graph::updateGraph(City& u, City& v)
{
	unique_lock<shared_mutex> lockGraph(mutexGraph_);
	auto edges = adj.find(u.id);

	cout << "Updating graph with DB values..." << endl;
	if (edges != adj.end())
	{
		adj.erase(u.id);
		for (auto it = adj.begin(); it != adj.end(); ++it)
		{
			// removes all values with id
			auto it2 = it->second.find(u.id);
			if (it2 != it->second.end())
			{
				it->second.erase(it2);
			}
		}

		cout << "Updating of graph completed." << endl << endl;
		return save();
	}
	else
	{
		cout << "Update of graph failed." << endl << endl;
	}

	return false;
}

// Prints shortest paths from src to all other vertexes 
string Graph::getShortestPath(int src, int dest)
{
	unique_lock<shared_mutex> lockGraph(mutexGraph_);
	const map<int, City>& cities = database.getCities();
	if (cities.find(src) == cities.end())
	{
		return "Shortest path calculation: starting city not found in DB.";
	}

	// Create a set to store vertexes that are being 
	// prerocessed 
	set< pair<int, int> > setds;

	// Create a map for distances
	map<int, int> dist;
	map<int, int> path;
	stack<int> orderedPath;

	for (auto id : adj)
	{
		dist[id.first] = INF;
		path[id.first] = -1;
	}
	dist[dest] = INF;

	// Insert source itself in Set and initialize its 
	// distance as 0. 
	setds.insert(make_pair(0, src));
	dist[src] = 0;

	/* Looping till all shortest distance are finalized
		then setds will become empty */
	while (!setds.empty())
	{
		// The first vertex in Set is the minimum distance 
		// vertex, extract it from set. 
		pair<int, int> tmp = *(setds.begin());
		setds.erase(setds.begin());

		// vertex label is stored in second of pair (it 
		// has to be done this way to keep the vertexes 
		// sorted distance (distance must be first item 
		// in pair) 
		int u = tmp.second;

		// 'it' is used to get all adjacent vertexes of a vertex 
		for (auto it = adj[u].begin(); it != adj[u].end(); ++it)
		{
			// Get vertex label and weight of current adjacent 
			// of u. 
			int v = *it;
			int weight = cities.find(v)->second.weight;

			//  If there is shorter path to v through u. 
			if (dist[v] > dist[u] + weight)
			{
				/*  If distance of v is not INF then it must be in
					our set, so removing it and inserting again
					with updated less distance.
					Note : We extract only those vertexes from Set
					for which distance is finalized. So for them,
					we would never reach here.  */
				if (dist[v] != INF)
					setds.erase(setds.find(make_pair(dist[v], v)));

				// Updating distance of v 
				dist[v] = dist[u] + weight;
				path[v] = u;
				setds.insert(make_pair(dist[v], v));
			}
		}
	}

	string result = "";

	if (dist[dest] != INF)
	{
		int srcWeight = database.getCities().find(src)->second.weight;
		// Return shortest distances stored in dist[] and path
		result += "Distance " + to_string(dist[dest]+srcWeight) + " ";

		int current = dest;
		while (current != src) {
			orderedPath.push(current);
			current = path[current];
		}

		result += "path ";
		while (!orderedPath.empty()) {
			result += to_string(orderedPath.top()) + " ";
			orderedPath.pop();
		}
	}
	else
	{
		result += "Path not found.";
	}

	return result;
}

// Save graph to file
bool Graph::save()
{
	cout << "Saving  into " << graphFileName << endl;
	ofstream outfile(graphFileName, ofstream::out | ofstream::trunc);
	// File doesn't exist
	if (outfile.fail())
	{
		cout << "File " << graphFileName << " does not exist." << endl;
		return false;
	}


	// iterate through multimap's elements (by key)
	for (auto it = adj.begin(); it != adj.end(); ++it)
	{
		// save
		for (auto it2 = it->second.begin() ; it2 != it->second.end(); ++it2)
		{
			outfile << to_string(it->first) << " ";
			outfile << to_string(*(it2)) << " " << endl;
		}
	}

	outfile.close();

	return true;
}

// Prints 
ostream& operator<<(ostream& os, const Graph& graph)
{
	os << "Printing graph...\n";
	if (graph.adj.size() == 0)
	{
		os << "The graph is empty";
	}
	else
	{
		// iterate through multimap's elements (by key)
		for (auto it = graph.adj.begin(); it != graph.adj.end(); ++it)
		{
			os << "Node " << it->first << " makes an edge with" << endl;

			// Get the range of the current key
			auto range = graph.adj.equal_range(it->first);

			// Now print out that whole range
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				os << "\tNode " << to_string(*(it2)) << " with edge" << endl;
			}
			os << "\n";
		}
	}
	return os;
}

