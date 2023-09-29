#include "Graph.h"

map<int, vector<pair<int, int>>> Graph::getAdjList() {
	return this->adjList;
}

vector<Edge> Graph::getEdgeList() {
	vector<Edge> edgesList;
	bool isWeighted = this->weighted;
	for (auto it = this->adjList.begin(); it != this->adjList.end(); ++it) {
		int f = it->first; vector<pair<int, int>> pairs = it->second;
		for (int i = 0; i < pairs.size(); i++) {
			edgesList.push_back(Edge(f, pairs[i].first, pairs[i].second, isWeighted));
		}
	}
	// удал€ем повтор€ющиес€ ребра в неориентированном графе
	if (!this->directed) {
		auto it_edge1 = edgesList.begin();
		while (it_edge1 != edgesList.end()) {
			auto it_edge2 = it_edge1 + 1;
			while (it_edge2 != edgesList.end()) {
				if (it_edge1->isReverse(*it_edge2)) {
					edgesList.erase(it_edge2);
					break;
				}
				it_edge2++;
			}
			it_edge1++;
		}
	}
	return edgesList;
}

bool Graph::isDirected() {
	return this->directed;
}

bool Graph::isWeighted() {
	return this->weighted;
}

void Graph::addEdge(int f, int s, int w) {
	if (this->vertexs.find(f) == this->vertexs.end() || this->vertexs.find(s) == this->vertexs.end())
		throw OperationErr("there is no such vertex(s)");

	vector<Edge> edges = this->getEdgeList();
	if (find(edges.begin(), edges.end(), Edge(f, s, w, this->weighted)) != edges.end())
		throw OperationErr("such edge already exists");

	this->adjList[f].push_back(make_pair(s, w));
	if (!this->directed)
		this->adjList[s].push_back(make_pair(f, w));
}

void Graph::addVertex(int v) {
	if (this->vertexs.find(v) != this->vertexs.end())
		throw OperationErr("vertex with such name already exists");

	this->vertexs.insert(v);
	this->adjList[v] = {};
}

void Graph::deleteVertex(int v) {
	if (this->vertexs.find(v) == this->vertexs.end())
		throw OperationErr("there is no such vertex");

	this->vertexs.erase(v);
	this->adjList.erase(v);
	for (auto adj_it = this->adjList.begin(); adj_it != this->adjList.end(); ++adj_it) {
		auto pair_it = adj_it->second.begin();
		while (pair_it != adj_it->second.end()) {
			if (pair_it->first == v) {
				(adj_it->second).erase(pair_it);
				pair_it = adj_it->second.begin();
				continue;
			}
			pair_it++;
		}
	}
}

void Graph::deleteEdge(int f, int s) {
	if (this->vertexs.find(f) == this->vertexs.end() || this->vertexs.find(s) == this->vertexs.end())
		throw OperationErr("there is no such vertex(s)");

	if (this->weighted)
		throw OperationErr("specify weight of the edge");

	vector<Edge> edges = this->getEdgeList();

	if (find(edges.begin(), edges.end(), Edge(f, s, false)) == edges.end())
		throw OperationErr("there is no such edge");

	auto it = find(this->adjList[f].begin(), this->adjList[f].end(), make_pair(s, 0));
	this->adjList[f].erase(it);
	if (!this->directed) {
		it = find(this->adjList[s].begin(), this->adjList[s].end(), make_pair(f, 0));
		this->adjList[s].erase(it);
	}
}

void Graph::deleteEdge(int f, int s, int w) {
	if (!this->weighted)
		throw OperationErr("graph is unweighted");

	if (this->vertexs.find(f) == this->vertexs.end() || this->vertexs.find(s) == this->vertexs.end())
		throw OperationErr("there is no such vertex(s)");

	vector<Edge> edges = this->getEdgeList();
	if (find(edges.begin(), edges.end(), Edge(f, s, w, true)) == edges.end())
		throw OperationErr("there is no such edge");

	auto it = find(this->adjList[f].begin(), this->adjList[f].end(), make_pair(s, w));
	this->adjList[f].erase(it);
	if (!this->directed) {
		it = find(this->adjList[s].begin(), this->adjList[s].end(), make_pair(f, w));
		this->adjList[s].erase(it);
	}
}

void Graph::printToFile(string fileName) {
	ofstream out(fileName);
	if (!out.is_open()) throw FileNameErr(fileName);

	string d = this->directed ? "directed" : "undirected";
	string w = this->weighted ? "weighted" : "unweighted";
	out << d << endl << w << endl;

	for (auto it = this->adjList.begin(); it != this->adjList.end(); it++) {
		out << it->first << ":";
		vector<pair<int, int>> v = it->second;
		for (int i = 0; i < v.size(); i++, out << ";") {
			out << " " << v[i].first;
			if (this->weighted)
				out << " (" << v[i].second << ")";
		}
		out << endl;
	}
	out.close();
}

int Graph::getStepIn(int f) {
	if (this->directed) {
		if (this->vertexs.find(f) == this->vertexs.end())
			throw OperationErr("there is no such vertex");

		int k = 0;
		for (auto it = this->adjList.begin(); it != this->adjList.end(); it++) {
			vector<pair<int, int>> v = it->second;
			for (int i = 0; i < v.size(); i++) {
				if (v[i].first == f) k++;
			}
		}
		return k;
	}
	else OperationErr("graph is not directed");
}

