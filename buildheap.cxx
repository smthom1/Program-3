#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using Key = int;
using Value = std::pair<int, int>;

class PriorityQueue {
 public:
  PriorityQueue(size_t maxSize) : maxHeapSize(maxSize) {}

  // Insert a key-value pair into the priority queue
  void insert(Key k, Value v) {
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
  void removeMin() {
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
  size_t getSize() const { return heap.size(); }

  // Get the maximum heap size
  size_t getMaxHeapSize() const { return maxHeapSize; }

  // Get the key-value pair at a given index
  std::pair<Key, Value> getKeyValuePair(size_t i) const { return heap[i]; }

 private:
  std::vector<std::pair<Key, Value>> heap;
  size_t maxHeapSize;
};

// Convert the priority queue to a JSON object
nlohmann::json jsonObj = nlohmann::json::object();
for (int i = 1; i <= pq.getSize(); i++) {
    nlohmann::json node = nlohmann::json::object();
    node["key"] = pq.getKeyAtIndex(i);
    if (pq.getParent(i) != 0) node["parent"] = pq.getParent(i);
    if (pq.getLeftChild(i) != 0) node["leftChild"] = pq.getLeftChild(i);
    if (pq.getRightChild(i) != 0) node["rightChild"] = pq.getRightChild(i);
    jsonObj[std::to_string(i)] = node;
}
nlohmann::json metadata = nlohmann::json::object();
metadata["maxHeapSize"] = maxHeapSize;
metadata["numOperations"] = numOperations;
metadata["size"] = pq.getSize();
jsonObj["metadata"] = metadata;

// Print the JSON object to stdout
std::cout << jsonObj.dump(2) << std::endl;

return 0;
