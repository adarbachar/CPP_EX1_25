
#define DOCTEST_CONFIG_NO_MULTITHREADING
#define DOCTEST_CONFIG_NO_COLOR_OUTPUT
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cerrno>
#include <ctime>

// Shim for gmtime_s on MinGW/Windows
inline int gmtime_s(std::tm* tmDest, const time_t* sourceTime) {
    if (!tmDest || !sourceTime) return EINVAL;
    std::tm* res = std::gmtime(sourceTime);
    if (res) { *tmDest = *res; return 0; }
    return -1;
}

#include "doctest.h"
#include <stdexcept>
#include "graph.hpp"
#include "Algorithms.hpp"
#include "QForAlg.hpp"
#include "MinHeap.hpp"
#include "VertexMinHeap.hpp"
#include "EdgeMinHeap.hpp"

namespace gr = graph;
namespace vertexHeap = vertexheap;
namespace edgeHeap = edgeheap;
namespace qQueue = QForAlg;

/// @brief Count undirected edges once (each stored twice internally)
static int countEdges(const gr::Graph& g) {
    int cnt = 0;
    for (int u = 0; u < g.numVertices; ++u) {
        for (gr::Edge* e = g.adjList[u]->edges; e; e = e->next) {
            if (u < e->dest->data)
                ++cnt;
        }
    }
    return cnt;
}

/// @brief Return degree (number of neighbors) of vertex u
static int vertexDegree(const gr::Graph& g, int u) {
    int d = 0;
    for (gr::Edge* e = g.adjList[u]->edges; e; e = e->next)
        ++d;
    return d;
}

TEST_CASE("Graph constructor and basic properties") {
    // empty graph has correct vertex count and no edges
    gr::Graph g(5);
    CHECK_EQ(g.numVertices, 5);
    CHECK_EQ(::countEdges(g), 0);
    for (int i = 0; i < g.numVertices; ++i)
        CHECK_EQ(vertexDegree(g, i), 0);
}

TEST_CASE("Add and remove edges: valid & invalid") {
    gr::Graph g(4);
    // valid adds
    g.addEdge(0, 1, 3);
    g.addEdge(1, 2, 5);
    CHECK_EQ(::countEdges(g), 2);
    CHECK_EQ(vertexDegree(g, 0), 1);
    CHECK_EQ(vertexDegree(g, 1), 2);
    CHECK_EQ(vertexDegree(g, 2), 1);
    CHECK_EQ(vertexDegree(g, 3), 0);

    // invalid indices
    CHECK_THROWS_AS(g.addEdge(-1, 1, 1), std::out_of_range);
    CHECK_THROWS_AS(g.addEdge(0, 4, 1),  std::out_of_range);

    // invalid weights
    CHECK_THROWS_AS(g.addEdge(0, 1, 0),  std::invalid_argument);
    CHECK_THROWS_AS(g.addEdge(0, 1, -2), std::invalid_argument);

    // remove non-existing
    CHECK_THROWS_AS(g.removeEdge(0, 2), std::runtime_error);

    // valid remove
    g.removeEdge(1, 2);
    CHECK_EQ(::countEdges(g), 1);
    CHECK_EQ(vertexDegree(g, 1), 1);
    CHECK_THROWS_AS(g.removeEdge(1, 2), std::runtime_error);
}

TEST_CASE("Union–Find operations") {
    // two unions, find roots
    gr::Graph g(3);
    gr::Vertex* a = g.adjList[0];
    gr::Vertex* b = g.adjList[1];
    gr::Vertex* c = g.adjList[2];

    CHECK(g.unionSets(a, b));
    CHECK_FALSE(g.unionSets(a, b));
    CHECK(g.find(a) == g.find(b));

    CHECK(g.unionSets(b, c));
    CHECK(g.find(c) == g.find(a));
}

TEST_CASE("Queue (QForAlg) functionality") {
    // enqueue/dequeue FIFO behavior
    qQueue::QForAlg q(3);
    CHECK(q.is_empty());
    CHECK_THROWS_AS(q.dequeue(), std::out_of_range);

    gr::Graph g(3);
    q.enqueue(g.adjList[0]);
    q.enqueue(g.adjList[1]);
    q.enqueue(g.adjList[2]);
    CHECK_FALSE(q.is_empty());
    CHECK(q.is_full());

    CHECK_EQ(q.dequeue()->data, 0);
    CHECK_EQ(q.dequeue()->data, 1);
    CHECK_EQ(q.dequeue()->data, 2);
    CHECK(q.is_empty());
}

TEST_CASE("VertexMinHeap operations") {
    // min-heap by distance
    vertexHeap::VertexMinHeap h;
    CHECK(h.isEmpty());
    CHECK_THROWS_AS(h.extractMin(), std::runtime_error);

    gr::Graph g(4);
    g.adjList[0]->distance = 7;
    g.adjList[1]->distance = 3;
    g.adjList[2]->distance = 5;
    g.adjList[3]->distance = 1;

    for (int i = 0; i < g.numVertices; ++i)
        h.insert(g.adjList[i]);
    CHECK_EQ(h.size(), 4);

    // expect order: 3,1,2,0
    CHECK_EQ(h.extractMin()->data, 3);
    CHECK_EQ(h.extractMin()->data, 1);
    CHECK_EQ(h.extractMin()->data, 2);
    CHECK_EQ(h.extractMin()->data, 0);
    CHECK(h.isEmpty());
}

TEST_CASE("BFS correctness") {
    // BFS tree has n-1 edges in a path graph
    gr::Graph g(5);
    CHECK_THROWS_AS(bfs(g, 5), std::out_of_range);

    for (int i = 0; i < 4; ++i)
        g.addEdge(i, i+1, 1);

    gr::Graph* tree = bfs(g, 0);
    CHECK_EQ(::countEdges(*tree), 4);
    delete tree;
}

TEST_CASE("DFS correctness") {
    // DFS forest covers all edges once
    gr::Graph g(5);
    CHECK_THROWS_AS(dfs(g, 5), std::out_of_range);

    for (int i = 0; i < 4; ++i)
        g.addEdge(i, i+1, 1);

    gr::Graph* forest = dfs(g, 2);
    CHECK_EQ(::countEdges(*forest), 4);
    delete forest;
}

TEST_CASE("Dijkstra correctness") {
    // shortest-path tree has n-1 edges
    gr::Graph g(4);
    g.addEdge(0,1,2);
    g.addEdge(1,2,2);
    g.addEdge(0,2,10);
    g.addEdge(2,3,1);

    gr::Graph* tree = dijkstra(g, 0);
    CHECK_EQ(::countEdges(*tree), 3);
    delete tree;
}

TEST_CASE("Prim correctness") {
    // MST on small graph
    gr::Graph g(4);
    (void)g.adjList[0]->edges; // suppress unused warning
    g.addEdge(0,1,4);
    g.addEdge(0,2,3);
    g.addEdge(1,2,1);
    g.addEdge(1,3,2);

    gr::Graph* mst = prim(g);
    CHECK_EQ(::countEdges(*mst), 3);
    delete mst;
}

TEST_CASE("Kruskal correctness") {
    // MST via edge-sorted union-find
    gr::Graph g(4);
    g.addEdge(0,1,4);
    g.addEdge(0,2,3);
    g.addEdge(1,2,1);
    g.addEdge(1,3,2);

    gr::Graph* mst = kruskal(g);
    CHECK_EQ(::countEdges(*mst), 3);
    delete mst;
}
