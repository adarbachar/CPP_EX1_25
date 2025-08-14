#include <iostream>
#include "graph.hpp"
#include "Algorithms.hpp"

namespace gr = graph;
namespace st = std;

int main() {
    st::cout << "=== Graph Algorithms Demo ===" << st::endl;

    // Create a sample graph
    gr::Graph g(8);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 3, 3);
    g.addEdge(1, 4, 1);
    g.addEdge(2, 5, 4);
    g.addEdge(2, 6, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(3, 7, 2);

    st::cout << "\nOriginal graph:" << st::endl;
    g.print_graph();
    st::cout << "\nOriginal graph (Visual):" << st::endl;
    g.print_graph_visually(&g, g.adjList[0]);

    // BFS
    try {
        gr::Graph* bfsTree = bfs(g, 0);
        st::cout << "\n=== BFS Tree from vertex 0 ===" << st::endl;
        bfsTree->print_graph();
        st::cout << "\nBFS Tree (Visual):" << st::endl;
        bfsTree->print_graph_visually(bfsTree, bfsTree->adjList[0]);
        delete bfsTree;
    } catch (const st::exception& e) {
        st::cout << "BFS Error: " << e.what() << st::endl;
    }

    // DFS
    try {
        gr::Graph* dfsTree = dfs(g, 0);
        st::cout << "\n=== DFS Tree from vertex 0 ===" << st::endl;
        dfsTree->print_graph();
        st::cout << "\nDFS Tree (Visual):" << st::endl;
        dfsTree->print_graph_visually(dfsTree, dfsTree->adjList[0]);
        delete dfsTree;
    } catch (const st::exception& e) {
        st::cout << "DFS Error: " << e.what() << st::endl;
    }

    // Dijkstra
    try {
        gr::Graph* dijkstraTree = dijkstra(g, 0);
        st::cout << "\n=== Dijkstra Tree from vertex 0 ===" << st::endl;
        dijkstraTree->print_graph();
        st::cout << "\nDijkstra Tree (Visual):" << st::endl;
        dijkstraTree->print_graph_visually(dijkstraTree, dijkstraTree->adjList[0]);
        delete dijkstraTree;
    } catch (const st::exception& e) {
        st::cout << "Dijkstra Error: " << e.what() << st::endl;
    }

    // Kruskal MST
    try {
        gr::Graph* kruskalMST = kruskal(g);
        st::cout << "\n=== MST (Kruskal) ===" << st::endl;
        kruskalMST->print_graph();
        st::cout << "\nMST (Kruskal - Visual):" << st::endl;
        kruskalMST->print_graph_visually(kruskalMST, kruskalMST->adjList[0]);
        delete kruskalMST;
    } catch (const st::exception& e) {
        st::cout << "Kruskal Error: " << e.what() << st::endl;
    }

    // Prim MST
    try {
        gr::Graph* primMST = prim(g);
        st::cout << "\n=== MST (Prim) ===" << st::endl;
        primMST->print_graph();
        st::cout << "\nMST (Prim - Visual):" << st::endl;
        primMST->print_graph_visually(primMST, primMST->adjList[0]);
        delete primMST;
    } catch (const st::exception& e) {
        st::cout << "Prim Error: " << e.what() << st::endl;
    }

    return 0;
}