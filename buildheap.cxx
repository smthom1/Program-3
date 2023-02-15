#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "json.hpp"
#include "priorityqueue.h"

using json = nlohmann::json;

// Expects input file path as first argument
int main(int argc, char** argv) {
    // Checks that a file path is given
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file1.json" << std::endl;
        return -1;
    }

    // Convert the priority queue to a JSON object
    std::ifstream jsonFile(argv[1]);
    // opens and reads input JSON, then constructs priority queue
    if(!jsonFile){
        std::cerr << "Error: Unable to open file" << std::endl;
        return -1;
    }
    json jsonBuild;
    jsonFile >> jsonBuild;


    size_t maxSize = jsonBuild["metadata"]["maxHeapSize"];
    PriorityQueue pq(maxSize);

    // Traverses through all operations from the json file
    for(auto itr = jsonBuild.begin(); itr != jsonBuild.end(); ++itr) {
        // Skips metadata
        if(itr.key() == "metadata"){
            continue;
        }
        if(jsonBuild[itr.key()]["operation"] == "insert"){
            // If operation is an insert, adds a new node to the priority queue
            pq.insert((Key)jsonBuild[itr.key()]["key"]);
        } else {
            // If operation is not insert, removes min node from priotity queue
            pq.removeMin();
        }
    }

    // Creates a json from priority queue, sets max size and operation count metadata
    json jsonObj = pq.JSON();
    jsonObj["metadata"]["maxHeapSize"] = jsonBuild["metadata"]["maxHeapSize"];
    jsonObj["metadata"]["numOperations"] = jsonBuild["metadata"]["numOperations"];
    // Prints resulting JSON object to standard output
    std::cout << jsonObj.dump(2) << std::endl;

    return 0;
}
