
#pragma once
#include "MinHeap.hpp"

namespace edgeheap {

// this is the HeapEdge
struct HeapEdge {
    int from, to, weight;
    HeapEdge(int f, int t, int w) : from(f), to(t), weight(w) {}
};

// comparator  for HeapEdge
struct EdgeCompare {
    bool operator()(const HeapEdge& a, const HeapEdge& b) const {
        return a.weight < b.weight;
    }
};

// alias the MinHeap<HeapEdge,EdgeCompare>
using EdgeMinHeap = MinHeap<HeapEdge, EdgeCompare>;

}