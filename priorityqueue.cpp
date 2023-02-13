#include <iostream>
#include "json.hpp"
#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	if (size_ >= max_size_) {
		return;
	}
	nodes_[++size_] = kv;
	heapifyUp(size_);
}

KeyValuePair PriorityQueue::min() {
	if (size_ == 0) {
		return KeyValuePair();
	}
	return nodes_[ROOT];
}

KeyValuePair PriorityQueue::removeMin() {
	if (size_ == 0) {
		return KeyValuePair();
	}
	KeyValuePair min = nodes_[ROOT];
	nodes_[ROOT] = nodes_[size_--];
	heapifyDown(ROOT);
	return min;
}

bool PriorityQueue::isEmpty() const {
	return size_ == 0;
}

size_t PriorityQueue::size() const {
	return size_;
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
	for (size_t i = 1; i <= size_; i++) {
		nlohmann::json node;
		KeyValuePair kv = nodes_[i];
		node["key"] = kv.first;
		node["value"] = kv.second;
		if (i != ROOT) {
			node["parent"] = std::to_string(i / 2);
		}
		if (2 * i <= size_) {
			node["leftChild"] = std::to_string(2 * i);
		}
		if (2 * i + 1 <= size_) {
			node["rightChild"] = std::to_string(2 * i + 1);
		}
		result[std::to_string(i)] = node;
	}
	result["metadata"]["max_size"] = max_size_;
	result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	if (i <= ROOT) {
		return;
	}
	size_t parent = i / 2;
	if (nodes_[i].first < nodes_[parent].first) {
		std::swap(nodes_[i], nodes_[parent]);
		heapifyUp(parent);
	}
}

void PriorityQueue::heapifyDown(size_t i) {
    //left and right child represent the indices of the children nodes
  size_t leftChild = 2 * i;
  size_t rightChild = 2 * i + 1;
  size_t smallest = i;
  if (leftChild <= size_ && getKey(leftChild) < getKey(i)) {
    smallest = leftChild;
  }
  if (rightChild <= size_ && getKey(rightChild) < getKey(smallest)) {
    smallest = rightChild;
  }
  if (smallest != i) {
    std::swap(nodes_[i], nodes_[smallest]);
    heapifyDown(smallest);
  }
}

Key PriorityQueue::getKey(size_t i) {
  return nodes_[i].first;
}

void PriorityQueue::removeNode(size_t i) {
  nodes_[i] = nodes_[size_];
  size_--;
  heapifyDown(i);
}

// Extra functions-----------------------------------------
/*
// Insert a key-value pair into the priority queue
void PriorityQueue::insert(Key k, Value v) {
    // Check if the queue is full
    if (heap.size() == maxHeapSize) {
        std::cout << "Priority queue is full. Cannot insert key." << std::endl;
        return;
    }

    // Add the key-value pair to the end of the queue
    heap.push_back({k, v});
    size_t i = heap.size() - 1;

    // Keep swapping the key with its parent until it is in the right place
    while (i > 0 && heap[(i - 1) / 2].first > heap[i].first) {
        std::swap(heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }
}


// Remove the minimum key from the priority queue
void PriorityQueue::removeMin() {
    // Check if the queue is empty
    if (heap.empty()) {
        std::cout << "Priority queue is empty. Cannot remove key." << std::endl;
        return;
    }

    // Move the last key to the front and pop it from the queue
    heap[0] = heap.back();
    heap.pop_back();
    size_t i = 0;

    // Keep swapping the key with the smaller of its children until it is in the right place
    while (true) {
        size_t leftChild = 2 * i + 1;
        size_t rightChild = 2 * i + 2;
        size_t minIndex = i;

        if (leftChild < heap.size() && heap[leftChild].first < heap[minIndex].first) {
            minIndex = leftChild;
        }

        if (rightChild < heap.size() && heap[rightChild].first < heap[minIndex].first) {
            minIndex = rightChild;
        }

        if (minIndex == i) {
            break;
        }

        std::swap(heap[i], heap[minIndex]);
        i = minIndex;
    }
}

// Get the current size of the priority queue
size_t PriorityQueue::getSize() const { return heap.size(); }
*/

// Get the maximum heap size
size_t PriorityQueue::getMaxHeapSize() const { return max_size_; }

// Get the key-value pair at a given index
std::pair<Key, Value> PriorityQueue::getKeyValuePair(size_t i) const { return nodes_[i]; }