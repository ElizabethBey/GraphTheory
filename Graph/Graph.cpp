#include <algorithm>
#include <stack>
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
	// удаляем повторяющиеся ребра в неориентированном графе
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

int Graph::getAmountStepIn(int f) {
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
	else throw OperationErr("graph is not directed");
}

vector<int> Graph::getVertexStepIn(int f) {
	if (this->directed) {
		if (this->vertexs.find(f) == this->vertexs.end())
			throw OperationErr("there is no such vertex");

		int k = 0;
		vector<int> stepIn;
		for (auto it = this->adjList.begin(); it != this->adjList.end(); it++) {
			vector<pair<int, int>> v = it->second;
			if (it->first != f)
				for (int i = 0; i < v.size(); i++)
					if (v[i].first == f) {
						stepIn.push_back(it->first);
						break;
					}
		}
		return stepIn;
	}
	else throw OperationErr("graph is not directed");
}

Graph Graph::makeCompleteGraph() {
	if (this->weighted) throw OperationErr("graph is weighted");

	Graph g1(this->directed, false);
	int k = 0;
	for (auto v : this->vertexs)
		g1.addVertex(v);
	
	for (auto it1 = this->vertexs.begin(); it1 != this->vertexs.end(); ++it1) {
		auto it2 = it1; it2++;
		while (it2 != this->vertexs.end()) {
			g1.addEdge(*it1, *it2);
			if (this->directed) g1.addEdge(*it2, *it1);
			it2++;
		}
	}
	return g1;
}

Graph Graph::makeCompleteGraph(int w) {
	if (!this->weighted) throw OperationErr("graph is not weighted");

	Graph g1(this->directed, true);
	int k = 0;
	for (auto v : this->vertexs)
		g1.addVertex(v);

	for (auto it1 = this->vertexs.begin(); it1 != this->vertexs.end(); ++it1) {
		auto it2 = it1; it2++;
		while (it2 != this->vertexs.end()) {
			g1.addEdge(*it1, *it2, w);
			if (this->directed) g1.addEdge(*it2, *it1, w);
			it2++;
		}
	}
	return g1;
}

bool Graph::findPathDFS(int u1, int u2, int v, bool* visited, vector<int>& ans) {
	visited[u1] = true;
	ans.push_back(u1);
	if (u1 == u2) return true;
	for (int i = 0; i < this->adjList[u1].size(); ++i) {
		int u = this->adjList[u1][i].first;
		if (u != v && !visited[u])
			if (this->findPathDFS(u, u2, v, visited, ans))
				return true;
	}
	ans.erase(ans.end() - 1);
	return false;
}

vector<int> Graph::findPath(int u1, int u2, int v) {
	if (this->vertexs.find(u1) == this->vertexs.end() ||
		this->vertexs.find(u2) == this->vertexs.end() ||
		this->vertexs.find(v) == this->vertexs.end())
		throw OperationErr("there is no such vertex(s)");
	bool* visited = new bool[this->vertexs.size()];
	for (int i = 0; i < this->vertexs.size(); ++i) visited[i] = false;
	vector<int> path;
	if (findPathDFS(u1, u2, v, visited, path)) return path;
	else throw OperationErr("there is no shuch way");
}

void Graph::invDFS(int u1, bool*& visited, int shift, vector<int>& ans) {
	if (visited[u1 - shift]) return;
	visited[u1 - shift] = true;
	ans.push_back(u1);
	for (int i = 0; i < this->adjList[u1].size(); ++i) {
		int u = this->adjList[u1][i].first;
		if (!visited[u - shift]) {
			invDFS(u, visited, shift, ans);
		}
	}
	return;
}

void Graph::componentDFS(int u1, int*& comp, int k, int shift) {
	if (comp[u1 - shift] != 0) return;
	comp[u1 - shift] = k;
	for (int i = 0; i < this->adjList[u1].size(); ++i) {
		int u = this->adjList[u1][i].first;
		if (comp[u - shift] == 0) {
			componentDFS(u, comp, k, shift);
		}
	}
	return;
}

int Graph::amountOfConnectedParts() {
	int n = this->vertexs.size();
	if (!this->directed) throw OperationErr("graph is not directed");
	if (!n) return 0;
	Graph g1(true, false);
	for (auto v : this->vertexs) g1.addVertex(v);
	for (auto it = this->adjList.begin(); it != this->adjList.end(); it++) {
		int u = it->first;
		vector<pair<int, int>> v = it->second;
		for (int i = 0; i < v.size(); i++) {
			g1.addEdge(v[i].first, u);
		}
	}
	bool* visited = new bool[n];
	for (int i = 0; i < n; ++i) visited[i] = false;
	vector<int> ans;
	int shift = *this->vertexs.begin();
	for (auto v : this->vertexs)
		g1.invDFS(v, visited, shift, ans);

	int k = 1;
	int* component = new int[n];
	for (int i = 0; i < n; ++i) component[i] = 0;
	vector<int> b;
	for (int i = ans.size() - 1; i >= 0; --i) {
		if (component[ans[i] - shift] == 0) {
			this->componentDFS(ans[i], component, k, shift);
			k++;
		}
	}
	return k;
}

Graph makeOnEdgeList(vector<Edge> edges, bool d, bool w) {
	Graph g(d, w);
	set<int> vertexes;
	for (auto e : edges) {
		vertexes.insert(e.first);
		vertexes.insert(e.second);
	}
	for (int v : vertexes) g.addVertex(v);
	for (auto e : edges) g.addEdge(e.first, e.second, e.weight);
	return g;
}

Graph Graph::carcass() {
	if (!this->weighted || this->directed)
		throw OperationErr("Graph has to be weighted and undirected");
	vector<Edge> edges = this->getEdgeList();
	sort(edges.begin(), edges.end());
	int n = this->adjList.size();
	vector<int> tree;
	for (int i = 0; i < n; ++i) tree.push_back(i);
	vector<Edge> ans;
	int shift = *this->vertexs.begin();
	for (auto e : edges) {
		int a = e.first, b = e.second; 
		if (tree[a - shift] != tree[b - shift]) {
			ans.push_back(e);
			int oldTree = tree[b - shift], newTree = tree[a - shift];
			for (int i = 0; i < n; ++i)
				if (tree[i] == oldTree) tree[i] = newTree;
		}
	}
	return makeOnEdgeList(ans, false, true);
}

void Deikstra(Graph& g, bool*& visited, vector<int>& dist, int vInd, int shift) {
	map<int, vector<pair<int, int>>> adj = g.getAdjList();
	for (auto l : adj[vInd + shift]) {
		if (!visited[l.first - shift] && dist[l.first - shift] > l.second + dist[vInd])
			dist[l.first - shift] = l.second + dist[vInd];
	}
	visited[vInd] = true;
}

pair<int, int> Graph::shortestWays(int u, int v1, int v2) {
	if (!this->weighted) throw OperationErr("Graph has to be weighted");
	int n = this->vertexs.size();
	//алг дейкстры
	bool* visited = new bool[n];  vector<int> dist(n);
	for (int i = 0; i < n; ++i) {
		visited[i] = false;
		dist[i] = 10000;
	}
	int shift = *this->vertexs.begin();
	int vMinInd = u - shift;
	dist[vMinInd] = 0;
	while (true) {
		Deikstra(*this, visited, dist, vMinInd, shift);
		int vMinOld = vMinInd;
		for (int i = 0; i < n; ++i)
			if (!visited[i]) vMinInd = i;
		if (vMinInd == vMinOld) break;
		for (int i = 0; i < n; ++i)
			if (!visited[i] && dist[i] < dist[vMinInd])
				vMinInd = i;
	}
	
	pair<int, int> p = make_pair(dist[v1 - shift], dist[v2 - shift]);
	if (p.first == 10000) throw OperationErr("no way from u to v1");
	if (p.second == 10000) throw OperationErr("no way from u to v2");
	return p;
}

int Graph::getRadius() {	
	if (this->directed) throw OperationErr("Graph has to be undirected");

	int n = this->vertexs.size();
	Graph g(false, true);
	set<int> vertexes;
	vector<Edge> thisEdges = this->getEdgeList();
	for (auto e : thisEdges) {
		vertexes.insert(e.first);
		vertexes.insert(e.second);
	}
	for (int v : vertexes) g.addVertex(v);
	for (auto e : thisEdges) g.addEdge(e.first, e.second, 1);

	//алгоритм Флойда Уоршелла
	vector<vector<int>> dist(n);
	for (int i = 0; i < n; ++i) {
		dist[i].resize(n);
		for (int j = 0; j < n; j++)
			dist[i][j] = 10000;
	}
	int shift = *this->vertexs.begin();
	vector<Edge> edges = g.getEdgeList();
	for (auto e : edges) {
		int fir = e.first - shift, sec = e.second - shift;
		dist[fir][sec] = e.weight;
		if (!this->directed)
			dist[sec][fir] = e.weight;
	}

	for (int i = 0; i < n; ++i)
		dist[i][i] = 0;
	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; ++j)
				if (dist[i][j] > dist[i][k] + dist[k][j])
					dist[i][j] = dist[i][k] + dist[k][j];

	vector<int> exentr(n);
	for (int j = 0; j < n; ++j)
		exentr[j] = *max_element(dist[j].begin(), dist[j].end());
	return *min_element(exentr.begin(), exentr.end());
}

void Graph::deeper (vector<int>& way, int u, int v, int tmpSum, vector<vector<int>> &ans) {
	for (auto el : this->adjList[u]) {
		int w = el.second;
		int vi = el.first;
		if (find(way.begin(), way.end(), vi) == way.end()) {
			tmpSum -= w;
			if (vi == v && tmpSum == 0) {
				way.push_back(vi);
				ans.push_back(way);
				way.erase(way.end() - 1);
			}
			else if (tmpSum > 0) {
				way.push_back(vi);
				deeper(way, vi, v, tmpSum, ans);
				way.erase(way.end() - 1);
			}
			tmpSum += w;
		}
	}
}

vector<vector<int>> Graph::kShortestWays(int u, int v, int k) {
	if (!this->weighted) throw OperationErr("Graph has to be weighted");
	if (!this->directed) throw OperationErr("Graph has to be directed");

	int n = this->vertexs.size();
	int n1 = n + 1;
	vector<int> dist(n, 10000);
	
	// алг Форда-Беллмана
	int shift = *this->vertexs.begin();
	dist[u - shift] = 0;
	vector<Edge> edges = this->getEdgeList();
	for (int i = 1; i <= n - 1; ++i)
		for (auto e : edges) {
			int u = e.first - shift; int v = e.second - shift;
			int w = e.weight;
			if (dist[u] != 10000 && dist[v] > dist[u] + w)
				dist[v] = dist[u] + w;
		}
	for (auto e : edges) {
		int u = e.first - shift; int v = e.second - shift;
		int w = e.weight;
		if (dist[u] != 10000 && dist[v] > dist[u] + w)
			throw OperationErr("Graph has negative loop");
	}
	/*for (int i = 0; i < n; i++)
		if (dist[i] < dist[i])
			throw OperationErr("Graph has negative loop");
		*/
	int vInd = v - shift;
	int sum = 10000;
	for (int i = 1; i < n; ++i)
		if (dist[vInd] < sum) sum = dist[vInd];
	if (sum == 10000) throw OperationErr("no way from u to v");

	vector<vector<int>> ans;
	vector<int> way;
	way.push_back(u);
	//пройтись посчитать сумму
	int temp = sum;
	deeper(way, u, v, sum, ans);
	if (k > ans.size()) 
		throw OperationErr("Graph has less then k min ways from u to v");
	vector<vector<int>> ansk;
	for (int i = 0; i < k; i++) ansk.push_back(ans[i]);
	return ansk;
}
