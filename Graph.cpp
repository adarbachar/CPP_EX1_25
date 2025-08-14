
#include "graph.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace st = std;

namespace graph {

/// @brief Construct a Graph with a given number of vertices.
/// @param vertices Number of vertices; must be >= 0.
/// @throws std::invalid_argument if vertices < 0.
Graph::Graph(int vertices)
    : root(nullptr), numVertices(vertices) {
    if (vertices < 0) {
        throw st::invalid_argument("Number of vertices cannot be negative");
    }
    adjList = new Vertex*[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        adjList[i] = new Vertex(i);
    }
}

// Destroy the Graph, freeing all allocated vertices and edges.
Graph::~Graph() {
    for (int i = 0; i < numVertices; ++i) {
        // Delete all edges for this vertex
        Edge* current = adjList[i]->edges;
        while (current) {
            Edge* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        // Delete the vertex
        delete adjList[i];
    }
    delete[] adjList;
}

// Add a directed edge from 'from' to 'to' with given weight.
void Graph::addDirectedEdge(int from, int to, int weight) {
    if (from < 0 || from >= numVertices || to < 0 || to >= numVertices) {
        throw st::out_of_range("Vertex index out of range");
    }
    Edge* newEdge = new Edge(weight, adjList[from], adjList[to]);
    newEdge->next = adjList[from]->edges;
    adjList[from]->edges = newEdge;
}

// Add an undirected edge between two vertices with given weight.

void Graph::addEdge(int indU, int indV, int weight1) {
    if (indU < 0 || indU >= numVertices || indV < 0 || indV >= numVertices) {
        throw st::out_of_range("Vertex index out of range");
    }
    if (weight1 <= 0) {
        throw st::invalid_argument("Weight must be a positive integer");
    }
    addDirectedEdge(indU, indV, weight1);
    addDirectedEdge(indV, indU, weight1);
}

// Remove an undirected edge between two vertices.
void Graph::removeEdge(int u, int v) {
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
        throw st::out_of_range("Vertex index out of range");
    }
    // Remove u -> v
    bool found1 = false;
    Edge* current = adjList[u]->edges;
    Edge* prev = nullptr;
    while (current) {
        if (current->dest->data == v) {
            if (prev) prev->next = current->next;
            else adjList[u]->edges = current->next;
            delete current;
            found1 = true;
            break;
        }
        prev = current;
        current = current->next;
    }
    // Remove v -> u
    bool found2 = false;
    current = adjList[v]->edges;
    prev = nullptr;
    while (current) {
        if (current->dest->data == u) {
            if (prev) prev->next = current->next;
            else adjList[v]->edges = current->next;
            delete current;
            found2 = true;
            break;
        }
        prev = current;
        current = current->next;
    }
    if (!found1 || !found2) {
        throw st::runtime_error("Edge does not exist");
    }
}

// Print adjacency list representation to standard output.
void Graph::print_graph() const {
    for (int i = 0; i < numVertices; ++i) {
        Vertex* v = adjList[i];
        st::cout << "Vertex " << v->data << ":";
        for (Edge* e = v->edges; e; e = e->next) {
            st::cout << " ---> " << e->dest->data << " (weight: " << e->weight << ")";
        }
        st::cout << st::endl;
    }
}

// Helper for visual printing: count descendants recursively
static int countDescendants(Vertex* node, bool* visited) {
    if (!node || visited[node->data]) return 0;
    visited[node->data] = true;
    int count = 1;
    for (Edge* e = node->edges; e; e = e->next) {
        count += countDescendants(e->dest, visited);
    }
    return count;
}

// Helper: draw nodes and connectors in a matrix
static void drawTreeMatrix(char** matrix, Vertex* node, int row, int col, int spacing,
                           int& maxRow, int maxRows, int maxCols, bool* visited) {
    if (!node || row >= maxRows || col < 0 || col >= maxCols || visited[node->data]) return;
    visited[node->data] = true;
    // Label
    st::string label = "[" + st::to_string(node->data) + "]";
    for (int i = 0; i < (int)label.size() && col + i < maxCols; ++i)
        matrix[row][col + i] = label[i];
    // Compute spacing and recurse on children
    
    int childCount = 0;
    for (Edge* e = node->edges; e; e = e->next)
        if (!visited[e->dest->data]) childCount++;
    int childIndex = 0;
    int childSpacing = st::max(4, spacing/2);
    int totalWidth = childSpacing * st::max(0, childCount-1);
    for (Edge* e = node->edges; e; e = e->next) {
        Vertex* child = e->dest;
        if (visited[child->data]) continue;
        int childCol = col - totalWidth/2 + childIndex*childSpacing;
        int arrowRow = row+1;
        if (arrowRow < maxRows) {
            int mid = col + label.size()/2;
            if (childCol < col) matrix[arrowRow][col-1] = '/';
            else if (childCol > col) matrix[arrowRow][col+label.size()] = '\\';
            else matrix[arrowRow][mid] = '|';
        }
        drawTreeMatrix(matrix, child, row+2, childCol, st::max(spacing/2,3), maxRow, maxRows, maxCols, visited);
        childIndex++;
    }
    maxRow = st::max(maxRow, row);
}

// Print the graph visually as a tree from given root.
// g Pointer to graph instance.

void Graph::print_graph_visually(Graph* g, Vertex* root) {
    if (!g || !root || !g->adjList) throw st::invalid_argument("Invalid graph or root");
    int maxRows = g->numVertices*2 + 5;
    int maxCols = g->numVertices*12 + 20;
    char** matrix = new char*[maxRows];
    for (int i = 0; i < maxRows; ++i) {
        matrix[i] = new char[maxCols];
        std::fill(matrix[i], matrix[i]+maxCols, ' ');
    }
    bool* visited = new bool[g->numVertices]();
    int maxRow = 0;
    drawTreeMatrix(matrix, root, 0, maxCols/2, maxCols/4, maxRow, maxRows, maxCols, visited);
    for (int i = 0; i <= maxRow+2; ++i) {
        for (int j = 0; j < maxCols; ++j) st::cout << matrix[i][j];
        st::cout << st::endl;
    }
    for (int i = 0; i < maxRows; ++i) delete[] matrix[i];
    delete[] matrix;
    delete[] visited;
}

// Find representative of vertex in union-find structure.

Vertex* Graph::find(Vertex* v) {
    if (!v->father) v->father = v;
    if (v->father != v) v->father = find(v->father);
    return v->father;
}

//  Union two sets containing u and v.
//  u First vertex.
///  v Second vertex.

bool Graph::unionSets(Vertex* u, Vertex* v) {
    Vertex* ru = find(u);
    Vertex* rv = find(v);
    if (ru == rv) return false;
    ru->father = rv;
    return true;
}

} 