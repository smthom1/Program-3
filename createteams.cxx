#include <iostream>
#include <fstream>
#include <vector>
#include "teamdata.h"
#include "priorityqueue.h"

using json = nlohmann::json;

bool playerInVector(int playerToFind, std::vector<int> teamVector);

int main(int argc, char** argv){
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file.json" << std::endl;
        return 1;
    }
    // Read the JSON data
    json data;
    try {
        std::ifstream jsonFile(argv[1]);
        jsonFile >> data;
    } catch (const std::exception& e) {
        std::cerr << "Error reading input JSON: " << e.what() << std::endl;
        return 1;
    }

    // Adds all possible 2-person team combinations to a priority queue
    auto combinationArray = data["teamStats"];
    PriorityQueue teamQueue(combinationArray.size());
    for(int i = 0; i < (int)combinationArray.size(); i++) {
        // Makes a pair with the winPercentage (key) and the current pair of teammembers (value) and adds information to the queue
        teamQueue.insert(std::make_pair(std::abs((double)combinationArray[i]["winPercentage"] - 50.0),
                                        std::make_pair((int)combinationArray[i]["playerOne"], (int)combinationArray[i]["playerTwo"])));
    }

    // Assign players to a team of vectors based on priority queue
    std::vector<std::pair<int, int>> finalTeams;
    //assigned vector contains all players already in a team
    std::vector<int> assigned;

    int numPlayers = data["metadata"]["numPlayers"];
    //creates all teams using a greedy algorithm
    while ((int)finalTeams.size() < numPlayers / 2) {
        //gets the team still in the priority queue with the closest win percentage to 50
        std::pair<int, int> team = teamQueue.min().second;
        teamQueue.removeMin();
        //Checks if both members aren't in a team yet
        if (!playerInVector(team.first, assigned) && !playerInVector(team.second, assigned)) {
            //adds members to a team, notes that both members have been assigned already
            finalTeams.push_back(team);
            assigned.push_back(team.first);
            assigned.push_back(team.second);
        }
    }

    // Output the resulting teams
    nlohmann::json output;
    output["teams"] = finalTeams;
    std::cout << output.dump(2) << std::endl;
    return 0;
}

bool playerInVector(int playerToFind, std::vector<int> teamVector){
    for(int player : teamVector){
        if(playerToFind == player){
            return true;
        }
    }
    return false;
}

/* Commented out this function as the program as is already doesn't look at such instances
void removeAllInstancesWithPlayer(int player, PriorityQueue& pq){
    //Search up: can you reference user made classes?
    for(int i = 0; i < (int)pq.size(); i++){

    }
}/
    // Load the data into the priority queue
    const auto& teamStats = data["teamStats"];
    for (const auto& stat : teamStats) {
        const int playerOne = stat["playerOne"];
        const int playerTwo = stat["playerTwo"];
        const double winPercentage = stat["winPercentage"];
        if (!usedPlayers[playerOne][playerTwo]) {
            teamQueue.push(std::make_pair(winPercentage, std::make_pair(playerOne, playerTwo)));
            usedPlayers[playerOne][playerTwo] = true;
            usedPlayers[playerTwo][playerOne] = true;
        }
    }

    // Create the teams
    while (teams.size() < numPlayers / 2) {
        const auto team = teamQueue.top();
        teamQueue.pop();

        const int playerOne = team.second.first;
        const int playerTwo = team.second.second;
        if (!usedPlayers[playerOne][playerTwo]) {
            teams.push_back(team.second);
            usedPlayers[playerOne][playerTwo] = true;
            for (int i = 0; i < numPlayers; i++) {
                usedPlayers[i][playerOne] = true;
                usedPlayers[i][playerTwo] = true;
            }
        }
    }

    // Output the teams
    json output;
    output["teams"] = json::array();
    for (const auto& team : teams) {
        output["teams"].push_back({team.first, team.second});
    }
    std::cout << output << std::endl;
    //std::cout << data.dump(2) << std::endl;
    return 0;
}*/
