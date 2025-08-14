
#pragma once

#include <stdexcept>
#include <new>

// MinHeap: generic minimum heap using raw memory and comparator
// T: element type (requires copy-constructible and equality)
// Compare: comparator functor (returns true if first < second)
template<typename T, typename Compare>
class MinHeap {
    // array of elements
    T*  data_;
    // current element count
    int size_;
    // allocated capacity
    int capacity_;
    // comparator functor
    Compare cmp_;

    

    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

// Move constructor
MinHeap(MinHeap&& other) noexcept: data_(other.data_), size_(other.size_), capacity_(other.capacity_), cmp_(other.cmp_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// Move assignment
MinHeap& operator=(MinHeap&& other) noexcept {
    if (this != &other) {
        // Destroy current elements
        for (int i = 0; i < size_; ++i)
            data_[i].~T();
        operator delete[](data_);

        // Steal data from other
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        cmp_ = other.cmp_;

        // Null out source
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;

}

// swap two values
    static void swapVals(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    // grow: double capacity when full


    void grow() {
        int newCap = (capacity_ == 0 ? 1 : capacity_ * 2);
        T* newData = static_cast<T*>(operator new[](newCap * sizeof(T)));
        for (int i = 0; i < size_; ++i) {
            new(&newData[i]) T(data_[i]);  // copy-construct
            data_[i].~T();                  // destroy old
        }
        operator delete[](data_);
        data_     = newData;
        capacity_ = newCap;
    }

    // siftUp: restore heap property upward from index
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmp_(data_[i], data_[p])) {
                swapVals(data_[i], data_[p]);
                i = p;
            } else break;
        }
    }

    // siftDown: restore heap property downward from index
    void siftDown(int i) {
        while (true) {
            int l = 2*i + 1, r = l + 1, best = i;
            if (l < size_ && cmp_(data_[l], data_[best])) best = l;
            if (r < size_ && cmp_(data_[r], data_[best])) best = r;
            if (best == i) break;
            swapVals(data_[i], data_[best]);
            i = best;
        }


    }

public:
    // constructor: optional comparator
    explicit MinHeap(Compare cmp = Compare())
      : data_(nullptr), size_(0), capacity_(0), cmp_(cmp) {}

    // destructor: destroy elements and free memory
    ~MinHeap() {
        for (int i = 0; i < size_; ++i)
            data_[i].~T();
        operator delete[](data_);
    }

    // isEmpty: true if no elements
    bool isEmpty() const { return size_ == 0; }
    // isFull: true if at capacity before grow
    bool isFull()  const { return size_ == capacity_; }

    // Insert a new element (copy)
    void insert(const T& v) {
        if (size_ == capacity_) grow();
        new(&data_[size_]) T(v);  // placement new
        siftUp(size_++);
    }

    // Remove and return the minimum element
    T extractMin() {
        if (size_ == 0) throw std::runtime_error("Heap is empty");
        T root = data_[0];
        data_[0].~T();
        --size_;
        if (size_ > 0) {
            new(&data_[0]) T(data_[size_]);
            data_[size_].~T();
            siftDown(0);
        }
        return root;
    }

    // Peek at min element without removing
    const T& peek() const {
        if (size_ == 0) throw std::runtime_error("Heap is empty");
        return data_[0];
    }

    // size: current number of elements
    int size() const { return size_; }

    // contains: true if v exists in heap (linear search)
    bool contains(const T& v) const {
        for (int i = 0; i < size_; ++i)
            if (data_[i] == v)
                return true;
        return false;
    }

    // decreaseKey: after external key decrease, restore heap
    void decreaseKey(const T& val) {
        for (int i = 0; i < size_; ++i) {
            if (data_[i] == val) {
                siftUp(i);
                return;
            }
        }
        throw std::runtime_error("decreaseKey: value not found");
    }
};
