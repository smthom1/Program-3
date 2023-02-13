#include <iostream>
#include <fstream>
#include <vector>
#include "teamdata.h"

using json = nlohmann::json;

int main(int argc, char** argv){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " file.json" << std::endl;
        return -1;
    }
    std::ifstream jsonFile(argv[1]);
    if(!jsonFile){
        std::cerr << "Error: Unable to open file" << std::endl;
        return -1;
    }
    json teamData;
    jsonFile >> teamData;
    int totalTeams = (teamData["metadata"]["numPlayers"]) / 2;
    int currentTeams = 0;
    while(currentTeams < totalTeams) {

        for(int)
    }
    std::cout << teamData.dump(2) << std::endl;
    return 0;
}