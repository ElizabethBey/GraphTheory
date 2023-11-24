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

void printVector(vector<int> v) {
    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << " ";
    cout << "\n";
}

int main() {
    string s = "16-1.txt";
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
            << "9 - get info about graph\n" << "10 - give amount of coming edges (task4)\n"
            << "11 - give list of coming edges (task11)\n"
            << "12 - get complete graph based on g (task1)\n" 
            << "13 - way from u1 to u2, not passing throught v (task 8)\n"
            << "14 - give the amount of strongly connected components of directed graph\n"
            << "15 - get carcass\n"
            << "16 - print lenghts of shortest ways from u to v1 and v2\n"
            << "17 - find radius of the graph\n";
        cin >> action;
        int fir, sec, third, weight;
        vector<int> v;
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
            case 10:
                // 10 - give amount of coming edges
                cout << "Enter the name of vertex (int): ";
                cin >> fir;
                cout << g.getAmountStepIn(fir) << "\n";
                break;
            case 11:
                // 11 - give list of coming edges
                cout << "Enter the name of vertex (int): ";
                cin >> fir;
                v = g.getVertexStepIn(fir);
                if (v.size()) printVector(v);
                else cout << "\nNo coming edges\n";
                break;
            case 12:
                // 12 - complete graph
                if (g.isWeighted()) {
                    cout << "weight: ";
                    cin >> weight;
                    printAdjList(g.makeCompleteGraph(weight));
                }
                else
                    printAdjList(g.makeCompleteGraph());
                break;
            case 13:
                // 13 - Найти путь, соединяющий вершины u1 и u2 и не проходящий через вершину v.
                cout << "Enter the names of vertexs u1, u2: ";
                cin >> fir >> sec;
                cout << "Enter name of vertex v: ";
                cin >> third;
                printVector(g.findPath(fir, sec, third));
                break;
            case 14:
                // 14 - Подсчитать количество сильно связных компонент орграфа
                cout << g.amountOfConnectedParts() << endl;
                break;
            case 15:
                // 15 - найти каркас графа по алг Краскала
                printAdjList(g.carcass());
                break;
            case 16:
            {
                // 16 - Вывести длины кратчайших путей от u до v1 и v2
                cout << "Enter vertex u: ";
                cin >> fir;
                cout << "Enter vertexs v1, v2: ";
                cin >> sec >> third;
                pair<int, int> p = g.shortestWays(fir, sec, third);
                cout << "shortest ways lenghts\n" << "u -> v1: " << p.first
                    << "\nu -> v2: " << p.second << "\n";
                break;
            }
            case 17:
                // 17 - Найти радиус графа — минимальный из эксцентриситетов вершин
                cout << g.getRadius() << "\n";
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