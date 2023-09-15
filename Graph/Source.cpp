#include <iostream>
#include "Graph.h"

using namespace std;

// печатает список смежности графа
void printAdjList(Graph g) {
    map<int, vector<pair<int, int>>> adj = g.getAdjList();
    cout << "Adjacency List" << endl;
    for (auto it = adj.begin(); it != adj.end(); it++) {
        cout << it->first << ":";
        vector<pair<int, int>> v = it->second;
        for (int i = 0; i < v.size(); i++, cout << ";") {
            cout << " " << v[i].first;
            if (g.isWeighted())
                cout << " (" << v[i].second << ")";
        }
        cout << endl;
    }
}

// печатает список рёбер графа
void printEdgeList(Graph g) {
    vector<Edge> edges = g.getEdgeList();
    string razd = g.isDirected() ? "->" : "-";
    cout << "Edges List" << endl;
    for (auto it = edges.begin(); it != edges.end(); it++, cout << endl) {
        cout << it->first << razd << it->second;
        if (it->isWeighted) cout << " (" << it->weight << ")";
    }
    cout << endl;
}

int main() {
    string s = "somefile.txt";
    // cout << "Enter fileName: ";
    // cin >> s;
    Graph g;
    try {
        g = Graph(s);
        cout << g;
        printAdjList(g);
    }
    catch (Err& e) {
        e.print();
    }
    
    int action;
    while (true) {
        cout << "Choose operation: " << endl;
        cout << "1 - print adjList\n" << "2 - print edgesList\n"
            << "3 - add vertex\n" << "4 - add edge\n"
            << "5 - delete vertex\n" << "6 - delete edge\n"
            << "7 - read graph from file\n" << "8 - print graph into file\n"
            << "9 - get info about graph\n";
        cin >> action;
        int fir, sec, weight;
        try {
            switch (action) {
            case 1:
                // 1 - print adjList
                printAdjList(g);
                break;
            case 2:
                // 2 - print edgesList
                printEdgeList(g);
                break;
            case 3:
                // 3 - add vertex
                cout << "Enter the name of vertex (int): ";
                cin >> fir;
                g.addVertex(fir);
                printAdjList(g);
                break;
            case 4:
                // 4 - add edge
                cout << "Enter edge: ";
                cin >> fir >> sec;
                g.addEdge(fir, sec);
                printAdjList(g);
                break;
            case 5:
                // 5 - delete vertex
                cout << "Enter the name of vertex (int): ";
                cin >> fir;
                g.deleteVertex(fir);
                printAdjList(g);
                break;
            case 6:
                // 6 - delete edge
                cout << "Enter the edge vertex vertex ";
                if (g.isWeighted()) {
                    cout << "weight: ";
                    cin >> fir >> sec >> weight;
                    g.deleteEdge(fir, sec, weight);
                }
                else {
                    cin >> fir >> sec;
                    g.deleteEdge(fir, sec);
                }
                printAdjList(g);
                break;
            case 7:
                // 7 - read graph from file
                cout << "Enter fileName: ";
                cin >> s;
                g = Graph(s);
                printAdjList(g);
                cout << "Completed\n";
                break;
            case 8:
                // 8 - print graph into file
                cout << "Enter fileName: ";
                cin >> s;
                g.printToFile(s);
                cout << "Completed\n";
                break;
            case 9:
                // 9 - get info about graph
                cout << g;
                break;
            default:
                cout << "\nHave no operation with such name\n";
                break;
            }
        }
        catch (Err& e) {
            e.print();
        }

        cout << "Continue? (1/0) ";
        cin >> action;
        if (!action) break;
    }
	return 0;
}