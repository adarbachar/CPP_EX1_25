


#pragma once
#include "graph.hpp"

namespace graph {
    // Algorithm functions
    Graph* bfs(const Graph& g, int source);
    Graph* dfs(const Graph& g, int source);
    Graph* dijkstra(const Graph& g, int source);
    Graph* prim(const Graph& g);
    Graph* kruskal(Graph& g);
    int getHeight(Vertex* vertex);
}