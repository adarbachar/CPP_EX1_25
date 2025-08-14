
#pragma once
#include <iostream>
#include <stdexcept>


namespace graph {

/// Forward-declare Vertex so Edge can use it.
struct Vertex;

/// @brief Graph edge node.
struct Edge {
    int     weight;
    Vertex* src;
    Vertex* dest;
    Edge*   next;

    Edge(int w, Vertex* s, Vertex* d, Edge* n = nullptr)
      : weight(w), src(s), dest(d), next(n) {}
};

/// @brief Graph vertex node.
struct Vertex {
    int     data;
    Edge*   edges;    // adjacency list head
    Vertex* next;     // for lists if needed
    Vertex* father;   // for union-find, Dijkstra, etc.
    int distance; // for shortest-path algorithms

    Vertex(int val):
     data(val), edges(nullptr), next(nullptr), father(nullptr), distance(0) {}
    Vertex(int val, Vertex* nxt):
       data(val), edges(nullptr), next(nxt), father(nullptr), distance(0) {}
};

/// @brief Undirected weighted graph.
class Graph {
private:
    Vertex* root;   

public:
    int numVertices;
    Vertex** adjList;

    explicit Graph(int vertices);
    ~Graph();

    void addDirectedEdge(int from, int to, int weight);
    void addEdge(int u, int v, int weight);
    void removeEdge(int u, int v);

    void print_graph() const;
    void print_graph_visually(Graph* g, Vertex* root);

    Vertex* find(Vertex* v);
    bool    unionSets(Vertex* u, Vertex* v);
};

/// @brief Count total undirected edges (each stored twice internally).
inline int countEdges(const Graph& g) {
    int cnt = 0;
    for (int u = 0; u < g.numVertices; ++u) {
        for (Edge* e = g.adjList[u]->edges; e; e = e->next) {
            if (u < e->dest->data) ++cnt;
        }
    }
    return cnt;
}

}
