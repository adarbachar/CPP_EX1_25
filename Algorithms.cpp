
#include "Algorithms.hpp"
#include "QForAlg.hpp"
#include "MinHeap.hpp"
#include "VertexMinHeap.hpp"
#include "EdgeMinHeap.hpp"
#include <stdexcept>
#include <iostream>
#include <climits>

namespace vertexHeap = vertexheap;
namespace edgeHeap = edgeheap;
namespace qQueue = QForAlg;
namespace st = std;
namespace gr = graph;

namespace graph {

    // BFS: builds directed BFS tree from source
    // @param g      Graph to traverse (undirected or directed)
    // @param source Start index of traversal (0-based)
    // @return       New graph representing the BFS tree (directed)
    Graph* bfs(const Graph& g, int source) {
        int n = g.numVertices;
        if (source < 0 || source >= n)
            throw st::out_of_range("bfs: source out of range");

        Graph* result = new Graph(n);
        qQueue::QForAlg queue(n);
        bool* visited = new bool[n]();

        // Mark the source and enqueue
        visited[source] = true;
        queue.enqueue(g.adjList[source]);

        // Process until queue is empty
        while (!queue.is_empty()) {
            Vertex* u = queue.dequeue();
            for (Edge* e = u->edges; e; e = e->next) {
                int v = e->dest->data;
                if (!visited[v]) {
                    visited[v] = true;
                    queue.enqueue(e->dest);
                    // Add edge to BFS tree
                    result->addDirectedEdge(u->data, v, e->weight);
                }
            }
        }

        delete[] visited;
        return result;
    }

    // DFSVisit: recursive helper for DFS
    // @param v   Current vertex in traversal
    // @param vis Visited flag array
    // @param res Result graph being built
    static void DFSVisit(Vertex* v, bool* vis, Graph* res) {
        vis[v->data] = true;
        for (Edge* e = v->edges; e; e = e->next) {
            if (!vis[e->dest->data]) {
                // Add tree edge and recurse
                res->addEdge(v->data, e->dest->data, e->weight);
                DFSVisit(e->dest, vis, res);
            }
        }
    }

    // DFS: builds DFS forest starting at source
    // @param g      Graph to traverse (undirected or directed)
    // @param source Start index for initial DFS (0-based)
    // @return       New graph representing the DFS forest (tree edges)
    Graph* dfs(const Graph& g, int source) {
        int n = g.numVertices;
        if (source < 0 || source >= n)
            throw st::out_of_range("dfs: source out of range");

        Graph* result = new Graph(n);
        bool* visited = new bool[n]{};

        // Start DFS from the given source
        DFSVisit(g.adjList[source], visited, result);
        // Continue DFS for any unvisited vertices
        for (int i = 0; i < n; ++i)
            if (!visited[i])
                DFSVisit(g.adjList[i], visited, result);

        delete[] visited;
        return result;
    }

    // relaxEdge: relaxes a single edge during Dijkstra
    // @param u    Current vertex being processed
    // @param e    Edge from u to v
    // @param heap Min-heap of vertices keyed by distance
    static void relaxEdge(Vertex* u, Edge* e, vertexHeap::VertexMinHeap& heap) {
        Vertex* v = e->dest;
        int alt = u->distance + e->weight;
        if (alt < v->distance) {
            // Update shorter path and adjust heap
            v->distance = alt;
            v->father   = u;
            heap.decreaseKey(v);
        }
    }

    // Dijkstra: shortest-paths tree via Dijkstra's algo
    // @param g      Weighted graph to process
    // @param source Start index (0-based) for source vertex
    // @return       New graph of shortest-paths tree (directed)
    Graph* dijkstra(const Graph& g, int source) {
        int n = g.numVertices;
        if (source < 0 || source >= n)
            throw st::out_of_range("dijkstra: source out of range");

        // Initialize all distances to infinity and clear fathers
        for (int i = 0; i < n; ++i) {
            g.adjList[i]->distance = INT_MAX;
            g.adjList[i]->father   = nullptr;
        }

        // Build a heap of all vertices
        vertexHeap::VertexMinHeap heap;
        for (int i = 0; i < n; ++i)
            heap.insert(g.adjList[i]);

        // Set source distance to zero and update heap
        g.adjList[source]->distance = 0;
        heap.decreaseKey(g.adjList[source]);

        // Main loop: extract min and relax outgoing edges
        bool* visited = new bool[n]{};
        while (!heap.isEmpty()) {
            Vertex* u = heap.extractMin();
            int uid = u->data;
            if (visited[uid]) continue;
            visited[uid] = true;
            for (Edge* e = u->edges; e; e = e->next)
                relaxEdge(u, e, heap);
        }
        delete[] visited;

        // Build result graph by following father pointers
        Graph* result = new Graph(n);
        for (int i = 0; i < n; ++i) {
            if (Vertex* p = g.adjList[i]->father) {
                int w = g.adjList[i]->distance - p->distance;
                result->addDirectedEdge(p->data, i, w);
            }
        }
        return result;
    }

    // Prim: MST via Prim's algo
    // @param g Weighted undirected graph
    // @return  New graph of minimum spanning tree
    Graph* prim(const Graph& g) {
        int n = g.numVertices;
        if (n == 0)
            throw st::invalid_argument("prim: empty graph");

        // Initialize all distances to infinity and clear fathers
        for (int i = 0; i < n; ++i) {
            g.adjList[i]->distance = INT_MAX;
            g.adjList[i]->father   = nullptr;
        }

        // Build a heap of all vertices
        vertexHeap::VertexMinHeap heap;
        for (int i = 0; i < n; ++i)
            heap.insert(g.adjList[i]);

        // Start from vertex 0
        g.adjList[0]->distance = 0;
        heap.decreaseKey(g.adjList[0]);

        bool* inMST = new bool[n]{};
        Graph* result = new Graph(n);
        // Extract and relax edges to grow MST
        while (!heap.isEmpty()) {
            Vertex* u = heap.extractMin();
            int uid = u->data;
            if (inMST[uid]) continue;
            inMST[uid] = true;

            if (u->father)
                result->addEdge(u->father->data, uid, u->distance);

            for (Edge* e = u->edges; e; e = e->next) {
                Vertex* v = e->dest;
                int w    = e->weight;
                if (!inMST[v->data] && w < v->distance) {
                    v->distance = w;
                    v->father   = u;
                    heap.decreaseKey(v);
                }
            }
        }
        delete[] inMST;
        return result;
    }

    // Kruskal: MST via Kruskal's algo
    // @param g Weighted undirected graph (modified by union-find)
    // @return  New graph containing edges of the MST

    Graph* kruskal(Graph& g) {
        int n = g.numVertices;
        if (n < 0)
            throw st::invalid_argument("kruskal: negative vertex count");
        if (n <= 1)
            return new Graph(n);

        // Reset father for union-find initialization
        for (int i = 0; i < n; ++i)
            g.adjList[i]->father = nullptr;

        // Build a heap of all unique edges
        edgeHeap::EdgeMinHeap heap;
        for (int u = 0; u < n; ++u) {
            for (Edge* e = g.adjList[u]->edges; e; e = e->next) {
                int v = e->dest->data;
                if (u < v)
                    heap.insert(edgeHeap::HeapEdge(u, v, e->weight));
            }
        }

        Graph* result = new Graph(n);
        int used = 0;

        // Extract edges in increasing order and union if no cycle
        while (!heap.isEmpty() && used < n - 1) {
            edgeHeap::HeapEdge minE = heap.extractMin();
            Vertex* U  = g.adjList[minE.from];
            Vertex* V = g.adjList[minE.to];
            if (g.unionSets(U, V)) {
                result->addEdge(minE.from, minE.to, minE.weight);
                ++used;
            }
        }

        return result;
    }

}
