/*
	Code Written by: Joseph Paveglio

	This code reads in edges from a text file "network.txt", then organizes these edges into a graph.
	Multiple functions can be performed on this graph, such as:
		-Finding the shortest path to all other nodes from a specified source node
		-Editing the weight of an edge, removing an edge, or removing a node entirely
		-Calculate the minimum spanning tree of the graph

*/

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <limits>

#define INFINITE 1000000
using namespace std;

// structure used to identify a vertex ID + name combo
struct VertexID
{
	int ID;
	string name;
};

// a structure to represent a weighted edge in graph
struct Edge
{
	int src, dest, weight;
	string srcName, destName;
};

// a structure to represent a connected, directed and weighted graph
struct Graph
{
	// V-> Number of vertices, E-> Number of edges
	int V, E;

	// graph is represented as a vector of edges.
	vector<Edge> edge;
};

// sorting function used to sort edges by source node number
bool sourceEdge(Edge e1, Edge e2)
{
	return (e1.src < e2.src);
}

// returns a vector of edges with all the unique elements from the edge vector
vector<VertexID> distinctEdges(Graph *graph)
{
	vector<int> tempID;
	vector<VertexID> tempList;
	VertexID copy;

	// assigns every possible vertex ID to the temp vector
	for (unsigned int i = 0; i < graph->edge.size(); i++)
	{
		tempID.push_back(graph->edge[i].src);
		tempID.push_back(graph->edge[i].dest);
	}

	// creates a new vector where each element is different
	set<int> s(tempID.begin(), tempID.end());
	tempID.assign(s.begin(), s.end());

	bool copied;
	for (unsigned int i = 0; i < tempID.size(); i++)
	{
		copied = false;
		for (unsigned int j = 0; j < graph->edge.size(); j++)
		{
			if (graph->edge[j].src == tempID[i])
			{
				copy.ID = graph->edge[j].src;
				copy.name = graph->edge[j].srcName;
				tempList.push_back(copy);
				copied = true;
				break;
			}
			if (graph->edge[j].dest == tempID[i])
			{
				copy.ID = graph->edge[j].dest;
				copy.name = graph->edge[j].destName;
				tempList.push_back(copy);
				copied = true;
				break;
			}
			if (copied)
			{
				break;
			}
		}
	}
	return tempList;
}

// creates a graph from a network file
Graph createGraph(string filename)
{
	// variables used in helping to extract data from the file and inserting it into the graph
	string srcName, destName;
	int srcID, destID;
	int weight;
	int edgeCount = 0;
	int verCount = 0;
	Graph graph;
	Edge temp;

	// opens a file called network
	ifstream network(filename);
	// if the file opens properly
	if (network.is_open())
	{
		// while not at the end of the file
		while (!network.eof())
		{
			// take data from each line in the format of
			// srcID(int) srcName(string) destID(int) destName(string) weight(int)
			network >> srcID;
			network >> srcName;
			network >> destID;
			network >> destName;
			network >> weight;

			//assigns each value to a temporary edge
			temp.src = srcID;
			temp.srcName = srcName;
			temp.dest = destID;
			temp.destName = destName;
			temp.weight = weight;
			// insert edge into the list of vectors, effectively creating a graph from a list of edges
			graph.edge.push_back(temp);
			// increase the edge count by one
			edgeCount++;
		}
		// close the file
		network.close();
	}

	// counts the amount of nodes in the graph
	verCount = distinctEdges(&graph).size();
	
	// assigns values to the graph objects V and E variables. needed to perform bellmanford
	graph.V = verCount;
	graph.E = edgeCount;

	return graph;
}


// The main function that finds shortest distances from src to all other
// vertices using Bellman-Ford algorithm.  The function also detects negative
// weight cycle
void BellmanFord(Graph* graph, int src)
{
	int V = graph->V;
	int E = graph->E;
	vector<int> dist;

	// Step 1: Initialize distances from src to all other vertices as INFINITE
	dist.resize(V, INFINITE);
	dist[src] = 0;

	// Step 2: Relax all edges |V| - 1 times. A simple shortest path from src
	// to any other vertex can have at-most |V| - 1 edges
	for (int i = 1; i <= (V - 1); i++)
	{
		for (int j = 0; j < E; j++)
		{
			int u = graph->edge[j].src;
			int v = graph->edge[j].dest;
			int weight = graph->edge[j].weight;
			if ((dist[u] + weight) < dist[v])
			{
				dist[v] = dist[u] + weight;
			}
		}
	}

	// Step 3: check for negative-weight cycles.  The above step guarantees
	// shortest distances if graph doesn't contain negative weight cycle.  
	// If we get a shorter path, then there is a cycle.
	for (int i = 0; i < E; i++)
	{
		int u = graph->edge[i].src;
		int v = graph->edge[i].dest;
		int weight = graph->edge[i].weight;
		if (dist[u] + weight < dist[v])
			printf("Graph contains negative weight cycle");
	}

	cout << INFINITE << " value indicates no connection to specified node." << endl;
	cout << setw(20) << "Vertex" << setw(20) << "Distance from Source" << endl;
	for (unsigned int i = 0; i < dist.size(); i++)
	{
		cout << setw(20) << i << setw(20) << dist[i] << endl;
	}

	return;
}

// function that alters a specified edge's weight
void weightChange(Graph *graph, int n, int newWeight)
{
	graph->edge[n].weight = newWeight;
}

// removes an edge from the graph
void removeEdge(Graph *graph, int n)
{
	graph->edge.erase(graph->edge.begin() + n);
	graph->E--;
}

// removes a vertex and all related edges from the graph
void removeVertex(Graph *graph, int n)
{
	bool removed = false;
	for (unsigned int i = 0; i < graph->edge.size(); i++)
	{
		if (removed)
		{
			i--;
			removed = false;
		}
		if (graph->edge[i].src == n || graph->edge[i].dest == n)
		{
			graph->edge.erase(graph->edge.begin() + i);
			graph->E--;
			removed = true;
		}
	}
}
// custom comparing algorithm for sorting edges by weight
bool krCompWeight(Edge e1, Edge e2)
{
	return (e1.weight < e2.weight);
}

// kruskal set checker
bool krSetChecker(vector < vector < int >> set, int a, int b)
{
	// spans entire set vector
	for (unsigned int i = 0; i < set.size(); i++)
	{
		// spans vector at pos i in set vector
		for (unsigned int j = 0; j < set[i].size(); j++)
		{
			// if a is found at pos i
			if (set[i][j] == a)
			{
				// span vector at pos i in set vector again
				for (unsigned int k = 0; k < set[i].size(); k++)
				{
					// if b is found
					if (set[i][k] == b)
					{
						return true;
					}
				}
			}
			// if b is found at pos i
			if (set[i][j] == b)
			{
				// span vector at pos i in set vector again
				for (unsigned int k = 0; k < set[i].size(); k++)
				{
					// if a is found
					if (set[i][k] == a)
					{
						return true;
					}
				}
			}
		}
	}
	// if no sets contained the two elements specified by a and b
	return false;
}

// function to alter set by joining two sets together
vector<vector<int>> joinSet(vector < vector < int >> set, int a, int b)
{
	int pos;

	// iterates through the set
	for (unsigned int i = 0; i < set.size(); i++)
	{
		for (unsigned int j = 0; j < set[i].size(); j++)
		{
			//if b is found, mark pos of i
			if (set[i][j] == b)
			{
				pos = i;
			}
		}
	}

	// iterates through the set
	for (unsigned int i = 0; i < set.size(); i++)
	{
		for (unsigned int j = 0; j < set[i].size(); j++)
		{
			// if a is found
			if (set[i][j] == a)
			{
				// push back whatever was at bs position into a
				for (unsigned int k = 0; k < set[pos].size(); k++)
				{
					set[i].push_back(set[pos][k]);
				}
				// erase the set at pos b
				set.erase(set.begin() + pos);
				// return the set with the newly joined subset
				return set;
			}
		}
	}
}

// Kruskal's Algorithm
void kruskals(Graph *graph)
{
	// temp list
	vector<Edge> temp = graph->edge;
	vector<VertexID> vList = distinctEdges(graph);
	vector<Edge> mst;
	vector<int> tInt;
	vector<vector<int>>set;
	vector<vector<int>>tempSet;

	// initialize the set vector
	for (unsigned int i = 0; i < vList.size(); i++)
	{
		tInt.push_back(vList[i].ID);
		set.push_back(tInt);
		tInt.clear();
	}

	// sort list by weight
	sort(temp.begin(), temp.end(), krCompWeight);

	// goes through sorted edge vector
	for (unsigned int i = 0; i < temp.size(); i++)
	{
		// if false, then add node to mst
		if (!krSetChecker(set, temp[i].src, temp[i].dest))
		{
			mst.push_back(temp[i]);
			tempSet = set;
			set = joinSet(tempSet, temp[i].src, temp[i].dest);
			tempSet.clear();
		}
	}

	// prints the MST
	cout << setw(20) << "Vertex Source" << setw(20) << "Name" << setw(20) << "Vertex Destination" << setw(20) << "Name" << setw(20) << "weight" << endl;
	// goes through MST
	for (unsigned int i = 0; i < mst.size(); i++)
	{
		cout << setw(20) << mst[i].src << setw(20) << mst[i].srcName << setw(20) << mst[i].dest << setw(20) << mst[i].destName << setw(20) << mst[i].weight << endl;
	}
}

// function to add a new vertex to the graph
void addVertex(Graph *graph)
{
	Edge temp;
	int num, weight;
	string name;
	vector<VertexID> vList = distinctEdges(graph);

	cout << " Please enter a name for this new node : " << endl;
	cin >> name;
	cout << " Please choose a destination node to connect to : " << endl;

	cout << setw(20) << "Destination Number" << setw(20) << "Destination Name" << endl;
	for (unsigned int i = 0; i < vList.size(); i++)
	{
		cout << setw(20) << vList[i].ID << setw(20) << vList[i].name << endl;
	}
	cout << " Enter the destination number you want to connect to : " << endl;
	cin >> num;
	cout << " Enter a weight for this connection : " << endl;
	cin >> weight;

	//sets all the temp edges parameters
	temp.src = graph->edge.back().src + 1;
	temp.srcName = name;
	temp.dest = num;
	temp.destName = vList[num].name;
	temp.weight = weight;

	//pushes the edge into the graph
	graph->V++;
	graph->E++;
	graph->edge.push_back(temp);
}

int main()
{
	system("mode con lines=30"); //Set Console Height
	system("mode con cols=132"); //Set Console Width 

	// vector used to display vertex list (ID + Name)
	vector<VertexID> verList;
	// creates the graph from named text file
	Graph graph = createGraph("network.txt");

	int ch, temp, temp2, count;
	while (1)
	{
		// sorts the edge list after every change
		sort(graph.edge.begin(), graph.edge.begin(), sourceEdge);
		// updates the verList after every change
		verList = distinctEdges(&graph);

		// menu system
		cout << endl << endl;
		cout << " \t\tNetwork Program Menu " << endl;
		cout << endl;
		cout << " 1. Use Bellman-Ford to Find the Shortest Path " << endl;
		cout << " 2. Display the Minimum Spanning Tree using Kruskal's Algorithm " << endl;
		cout << " 3. Change the Weight of an Edge " << endl;
		cout << " 4. Remove an Edge from the Graph " << endl;
		cout << " 5. Remove a Vertex from the Graph " << endl;
		cout << " 6. Add a Vertex to the Graph " << endl;
		cout << " 7. Exit " << endl;
		cout << " Enter your choice : ";
		cin >> ch;
		switch (ch)
		{
		case 1:
			system("cls");
			cout << endl;
			cout << " Enter the Vertex you want to use as the Source :\n Nodes available to use as source: \n";
			cout << setw(20) << "Node Number" << setw(20) << "Name" << endl;
			for (unsigned int i = 0; i < verList.size(); i++)
			{
				cout << setw(20) << verList[i].ID << setw(20) << verList[i].name << endl;
			}
			cout << " Type in -1 to exit this option" << endl;
			cout << " Enter your node choice :" << endl;
			cin >> temp;
			if (temp > -1 && temp < verList.size())
			{
				BellmanFord(&graph, temp);
			}
			else 
			{
				cout << "Please try again." << endl;
				system("pause");
				system("cls");
				break;
			}
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			cout << " Minimum Spanning Tree - Kruskal's Algorithm : " << endl;
			kruskals(&graph);
			system("pause");
			system("cls");
			break;
		case 3:
			system("cls");
			cout << endl;
			cout << " Select which edge whose weight you want to change : " << endl;
			cout << setw(20) << "Edge" << setw(20) << "Source" << setw(20) << "Destination" << setw(20) << "Weight" << endl;
			count = 0;
			for (int i = 0; i < graph.edge.size(); i++)
			{
				cout << setw(20) << i << setw(20) << graph.edge[i].srcName << setw(20) << graph.edge[i].destName << setw(20) << graph.edge[i].weight << endl;
				count++;
				if (count > 20)
				{
					cout << "Continue to show more." << endl;
					system("pause");
					count = 0;
				}
			}
			cout << " Type in -1 to exit this option" << endl;
			cout << " Enter your edge choice : " << endl;
			cin >> temp;
			if (temp > -1 && temp < graph.edge.size())
			{
				cout << " Enter the new weight : " << endl;
				cin >> temp2;
				weightChange(&graph, temp, temp2);
				cout << " The weight has been changed." << endl;
			}
			else
			{
				cout << "Please try again." << endl;
				system("pause");
				system("cls");
				break;
			}
			system("pause"); 
			system("cls");
			break;
		case 4:
			system("cls");
			cout << endl;
			cout << " Select which edge you want to remove from the graph : " << endl;
			cout << setw(20) << "Edge" << setw(20) << "Source" << setw(20) << "Destination" << setw(20) << "Weight" << endl;
			count = 0;
			for (int i = 0; i < graph.edge.size(); i++)
			{
				cout << setw(20) << i << setw(20) << graph.edge[i].srcName << setw(20) << graph.edge[i].destName << setw(20) << graph.edge[i].weight << endl;
				count++;
				if (count > 20)
				{
					cout << "Continue to show more." << endl;
					system("pause");
					count = 0;
				}
			}
			cout << " Type in -1 to exit this option" << endl;
			cout << " Enter your edge choice : " << endl;
			cin >> temp;
			if (temp > -1 && temp < graph.edge.size())
			{
				removeEdge(&graph, temp);
				cout << " The edge has been removed." << endl;
			}
			else
			{
				cout << "Please try again." << endl;
				system("pause");
				system("cls");
				break;
			}
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			cout << endl;
			cout << " Select which vertex you want to remove from the graph : " << endl;
			cout << setw(20) << "Node Number" << setw(20) << "Name" << endl;
			for (int i = 0; i < verList.size(); i++)
			{
				cout << setw(20) << verList[i].ID << setw(20) << verList[i].name << endl;
			}
			cout << " Type in -1 to exit this option" << endl;
			cout << " Enter your vertex choice : " << endl;
			cin >> temp;
			if (temp > -1 && temp < verList.size())
			{
				removeVertex(&graph, temp);
				cout << " The vertex and all related edges have been removed." << endl;
			}
			else 
			{
				cout << "Please try again." << endl;
				system("pause");
				system("cls");
				break;
			}
			system("pause");
			system("cls");
			break;
		case 6:
			addVertex(&graph);
			system("pause");
			system("cls");
			break;
		case 7:
			return 0;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}