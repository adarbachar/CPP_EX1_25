#include "QForAlg.hpp"
#include <stdexcept>



namespace QForAlg {

// Simple FIFO queue for Vertex pointers.
QForAlg::QForAlg(int cap)
    : rear(nullptr), front(nullptr), size(0), capacity(cap) {}

// Free all queued nodes.
QForAlg::~QForAlg() {
    while (!is_empty()) dequeue();
}

// Add vertex to the back; can throw if full.
void QForAlg::enqueue(graph::Vertex* vertex) {
    if (is_full()) throw std::overflow_error("Queue is full");
    auto* node = new QueueEdge(vertex);
    if (!rear) front = rear = node;
    else {
        rear->next = node;
        rear = node;
    }
    ++size;
}

// Remove and return front vertex; can throw if empty.
graph::Vertex* QForAlg::dequeue() {
    if (is_empty()) throw std::out_of_range("Queue is empty");
    auto* node = front;
    graph::Vertex* v = node->data;
    front = front->next;
    if (!front) rear = nullptr;
    delete node;
    --size;
    return v;
}

// True if queue has no elements.
bool QForAlg::is_empty() const { return !front; }

// True if queue reached capacity.
bool QForAlg::is_full() const { return size == capacity; }

} 
