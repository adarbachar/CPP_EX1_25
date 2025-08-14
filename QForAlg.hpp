



#pragma once
#include "graph.hpp"


namespace QForAlg {

/// @brief Single node in the queue, holding a graph vertex.
struct QueueEdge {
    graph::Vertex* data; ///< Stored vertex
    QueueEdge*    next; ///< Next node in the queue

    /// @brief Construct a new QueueEdge
    /// @param val Pointer to the vertex to store
    QueueEdge(graph::Vertex* val)
        : data(val), next(nullptr) {}
};

/// @brief Fixed-capacity FIFO queue for graph vertices.
class QForAlg {
private:
    QueueEdge* rear;    
    QueueEdge* front;    
    int        size;   
    int        capacity; 

public:
    /// @brief Create a queue with given capacity.
    /// @param cap Maximum number of vertices that can be enqueued.
    explicit QForAlg(int cap);

    /// @brief Destroy the queue and free all nodes.
    ~QForAlg();

    /// @brief Enqueue a vertex at the back of the queue.
    /// @param vertex Pointer to the graph vertex to add.
    void enqueue(graph::Vertex* vertex);

    /// @brief Dequeue the front vertex.
    /// @return Pointer to the removed vertex.
    graph::Vertex* dequeue();

    /// @brief Check if the queue is empty.
    /// @return true if no elements are stored.
    bool is_empty() const;

    /// @brief Check if the queue is full.
    /// @return true if current size equals capacity.
    bool is_full() const;
};

}