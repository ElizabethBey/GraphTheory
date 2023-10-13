#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#pragma once
class Err
{
public:
	virtual void print() { cerr << "File name error\n"; }
};

class FileFormatErr : public Err
{
	string what;
public:
	FileFormatErr(string str) : what(str) {}
	void print() { cerr << "Wrong file format: " << what << endl; }
};

class FileNameErr : public Err
{
	string name;
public:
	FileNameErr(string str) : name(str) {}
	void print() { cerr << "Couldn't find file: " << name << endl; }
};

class OperationErr : public Err
{
	string what;
public:
	OperationErr(string str) : what(str) {}
	void print() { cerr << "Incorrect operation: " << what << endl; }
};

class Edge
{
public:
	int first, second;
	int weight;
	bool isWeighted;
	Edge(int a, int b, int w, bool isW) : first(a), second(b), weight(w), isWeighted(isW) {}
	Edge(int a, int b, bool isW) : first(a), second(b), weight(0), isWeighted(isW) {}
	// перегрузка оператора ==
	bool operator==(const Edge& e) {
		return (first == e.first && second == e.second && weight == e.weight && isWeighted == e.isWeighted);
	}
	bool isReverse(const Edge& e) {
		return (first == e.second && second == e.first && weight == e.weight && isWeighted == e.isWeighted);
	}
};

class Graph
{
public:
	Graph() {
		this->directed = false;
		this->weighted = false;
		this->adjList = {};
		this->vertexs = {};
	}

	Graph(bool d, bool w) {
		this->directed = d;
		this->weighted = w;
		this->adjList = {};
		this->vertexs = {};
	}

	Graph(const string fileName) {
		std::ifstream in;
		in.open(fileName);
		if (!in.is_open()) throw Err();

		/* создание графа из файла
		стр1 - ориентированный, неориентированный
		стр2 - взвешенный, невзвешенный
		далее стр списка смежности с весом, если граф взвешенный, или без него
		вершина: вершина (вес), вершина (вес), ...*/
		string s;
		getline(in, s);
		if (s == "directed") this->directed = true;
		else if (s == "undirected") this->directed = false;
		else throw FileFormatErr("graph type not specified: directed/undirected");

		getline(in, s);
		if (s == "weighted") this->weighted = true;
		else if (s == "unweighted") this->weighted = false;
		else throw FileFormatErr("graph type not specified: weighted/unweighted");
		
		/*
		directed/undirected
		weighted/unweighted
		vertex: vertex (weight); vertex (weight); ...
		or
		vertex: vertex; vertex; ...
		*/

		int firstV, secondV, weight = 0;
		vector<pair<int, int>> c;
		try {
			while (getline(in, s)) {
				int pos = s.find(": ");
				firstV = stoi(s.substr(0, pos));
				this->vertexs.insert(firstV);
				this->adjList[firstV] = c;
				s.erase(0, pos + 2);

				if (this->weighted) {
					pos = s.find(" ");
					while (pos != std::string::npos) {
						secondV = stoi(s.substr(0, pos));
						s.erase(0, pos + 1);
						pos = s.find(")");
						weight = stoi(s.substr(1, pos));
						this->adjList[firstV].push_back(make_pair(secondV, weight));
						s.erase(0, pos + 3);
						pos = s.find(" ");
					}
				}
				else {
					pos = s.find(";");
					while (pos != std::string::npos) {
						secondV = stoi(s.substr(0, pos));
						s.erase(0, pos + 2);
						this->adjList[firstV].push_back(make_pair(secondV, weight));
						pos = s.find(";");
					}
				}
			}
		}
		catch (...) {
			throw FileFormatErr("incorrect adjList format");
		}
		
		in.close();
	}

	Graph(const Graph& g) {
		adjList = g.adjList;
		vertexs = g.vertexs;
		directed = g.directed;
		weighted = g.weighted;
	}

	ostream& print(ostream& out) {
		string d = this->directed ? "yes" : "no";
		string w = this->weighted ? "yes" : "no";
		return out << "Graph info\tdirected: " << d << "\tweighted: " << w << endl;
	}

	friend ostream& operator<< (ostream& out, Graph& p) {
		return p.print(out);
	}

	map<int, vector<pair<int, int>>> getAdjList(); // возвращает список смежности
	vector<Edge> getEdgeList(); // возвращает список ребер
	bool isDirected(); // €вл€етс€ ли ориентированным
	bool isWeighted(); // €вл€етс€ ли взвешенным

	void addEdge(int f, int s, int w = 0); // добавл€ет ребро
	void addVertex(int s); // добавл€ет вершину
	void deleteVertex(int v); // удал€ет вершину
	void deleteEdge(int f, int s); // удал€ет ребро у невзвешенного графа
	void deleteEdge(int f, int s, int w); // удал€ет ребро у взвешенного
	void printToFile(string fileName); // печатает данные о графе в файл
	int getAmountStepIn(int v); // количество дуг, вход€щих в вершину
	vector<int> getVertexStepIn(int v); // дает список Ђзаход€щихї соседних вершин
	Graph makeCompleteGraph(); // полный граф на основе данного невзвешенного графа
	Graph makeCompleteGraph(int w); // полный граф на основе данного взвешенного графа
	bool mDFS(int u1, int u2, int v, bool* visited, vector<int>& ans); // рукурсивна€ фунци€ дл€ обхода в глубину
	vector<int> findPath(int u1, int u2, int v); // список обхода в глубину

private:
	bool directed;
	bool weighted;
	/* вершина - вектор пар,
	где first - смежна€ с ней вершина,
	second - вес ребра */
	map<int, vector<pair<int, int>>> adjList;
	set<int> vertexs;
};

