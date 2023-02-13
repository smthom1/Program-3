#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "json.hpp"
#include "priorityqueue.h"


using json = nlohmann::json;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file1.json" << std::endl;
        return -1;
    }

    // Convert the priority queue to a JSON object
    std::ifstream jsonFile(argv[1]);
    if(!jsonFile){
        std::cerr << "Error: Unable to open file" << std::endl;
        return -1;
    }
    json jsonBuild;
    jsonFile >> jsonBuild;


    size_t maxSize = jsonBuild["metadata"]["maxHeapSize"];
    PriorityQueue pq(maxSize);

    //Traverses through all operations from the json file
    for(auto itr = jsonBuild.begin(); itr != jsonBuild.end(); ++itr) {
        //Skips metadata
        if(itr.key() == "metadata"){
            continue;
        }
        if(jsonBuild[itr.key()]["operation"] == "insert"){
            pq.insert((Key)jsonBuild[itr.key()]["key"]);
        } else { //Automatically removes min
            pq.removeMin();
        }
    }

    //Creates a json from priority queue
    json jsonObj = pq.JSON();
    jsonObj["metadata"]["maxHeapSize"] = jsonBuild["metadata"]["maxHeapSize"];
    jsonObj["metadata"]["numOperations"] = jsonBuild["metadata"]["numOperations"];
    // Print the JSON object to stdout
    std::cout << jsonObj.dump(2) << std::endl;

    return 0;
}