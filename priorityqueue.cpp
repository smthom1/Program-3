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

// Inserting key-value pair into priority queue
void PriorityQueue::insert(KeyValuePair kv) {
	if (size_ >= max_size_) {   // if queue is already at max size, do nothing
		return;
	}
	nodes_[++size_] = kv;       // insert key-value pair into next available node
	heapifyUp(size_);           // reorder nodes so min-heap property is preserved
}

// Returning min key-value pair
KeyValuePair PriorityQueue::min() {
	if (size_ == 0) {
		return KeyValuePair();
	}
	return nodes_[ROOT];
}

// Heapifying priority queue after removing min pair
KeyValuePair PriorityQueue::removeMin() {
  // If priority queue is empty, return a default-constructed key-value pair
	if (size_ == 0) {                   
		return KeyValuePair();
	}

  // Save current minimum key-value pair and replace it with the last key-value pair
	KeyValuePair min = nodes_[ROOT];
	nodes_[ROOT] = nodes_[size_--];
	heapifyDown(ROOT);                  // Restore the heap property by moving the new root down the tree to its correct position
	return min;                         // return min key-value pair
}

// Given that the queue is empty, returns size of 0
bool PriorityQueue::isEmpty() const {
	return size_ == 0;
}

// Given that there is content in the queue, return the size
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

// Heapifying queue up from a given index
void PriorityQueue::heapifyUp(size_t i) {
	if (i <= ROOT) {                                // Base case: root node reached
		return;
	}
	size_t parent = i / 2;                          // Find index of parent node
	if (nodes_[i].first < nodes_[parent].first) {   // Swap node with parent if violates heap property
		std::swap(nodes_[i], nodes_[parent]);
		heapifyUp(parent);                      // Recursively continue to heapify up
	}
}

// Heapifying priority queue down from given index
void PriorityQueue::heapifyDown(size_t i) {
  // Left and right child represent the indices of the children nodes
  size_t leftChild = 2 * i;
  size_t rightChild = 2 * i + 1;

  // Find smallest of current node and children
  size_t smallest = i;              
  if (leftChild <= size_ && getKey(leftChild) < getKey(i)) {
    smallest = leftChild;
  }
  if (rightChild <= size_ && getKey(rightChild) < getKey(smallest)) {
    smallest = rightChild;
  }

  // If current node is not smallest, swap with smallest and recurse on smallest
  if (smallest != i) {
    std::swap(nodes_[i], nodes_[smallest]);
    heapifyDown(smallest);
  }
}

// Returns key of node at specified index, accessing 'first' member stored in nodes_
Key PriorityQueue::getKey(size_t i) {
  return nodes_[i].first;
}

// Removes node at index, first swapping node i with last node in heap at size_
void PriorityQueue::removeNode(size_t i) {
  nodes_[i] = nodes_[size_];
  size_--;
  heapifyDown(i);
}

// Get the maximum heap size
size_t PriorityQueue::getMaxHeapSize() const { 
  return max_size_; 
}

// Get the key-value pair at a given index
std::pair<Key, Value> PriorityQueue::getKeyValuePair(size_t i) const { 
  return nodes_[i]; 
}
