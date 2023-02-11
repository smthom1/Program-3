#include <iostream>
#include "json.hpp"
#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	// [prewritten]
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	// included insert func
	if (size_ >= max_size_) {
		return;
	}
	nodes_[++size_] = kv;
	heapifyUp(size_);
}

// Edits begin here -------
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

// Edits end here ---------

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

// --- incomplete from here on ---
KeyValuePair PriorityQueue::removeMin() {
  KeyValuePair result = nodes_[ROOT];
  nodes_[ROOT] = nodes_[size_];
  size_--;
  heapifyDown(ROOT);
  return result;
}

bool PriorityQueue::isEmpty() const {
  return size_ == 0;
}

size_t PriorityQueue::size() const {
  return size_;
}

Key PriorityQueue::getKey(size_t i) {
  return nodes_[i].first;
}

void PriorityQueue::removeNode(size_t i) {
  nodes_[i] = nodes_[size_];
  size_--;
  heapifyDown(i);
}

/*
void PriorityQueue::heapifyUp(size_t i) {
	// TODO: complete this function
}

void PriorityQueue::heapifyDown(size_t i) {
	// TODO: complete this function
}

void PriorityQueue::removeNode(size_t i) {
	// TODO: complete this function
}

Key PriorityQueue::getKey(size_t i) {
	// TODO: complete this function
}
*/
