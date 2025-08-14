#pragma once


#include <stdexcept>
#include <iostream>
#include "graph.hpp"
#include "MinHeap.hpp"


namespace vertexheap {

/// Comparator that orders Vertex* by their `distance` field.
struct VertexCompare {
    bool operator()(graph::Vertex* a, graph::Vertex* b) const {
        return a->distance < b->distance;
    }
};

/// A min‐heap of Vertex* keyed by their `distance`.
using VertexMinHeap = MinHeap<graph::Vertex*, VertexCompare>;

} 
