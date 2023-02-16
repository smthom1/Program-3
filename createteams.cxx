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

/* VERS. 2
#include <iostream>
#include <fstream>
#include <vector>
#include "teamdata.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " team_data.json\n";
    return 1;
  }

  // Load team data
  std::string filename(argv[1]);
  TeamData teamData(filename);
  int numPlayers = teamData.numPlayers();

  // Priority queue to sort the 2-person teams by the difference from 50% win rate
  std::priority_queue<std::pair<double, std::pair<int, int>>,
                      std::vector<std::pair<double, std::pair<int, int>>>,
                      std::greater<std::pair<double, std::pair<int, int>>>>
      teamQueue;

  // Add all 2-person teams to the priority queue
  for (int i = 0; i < numPlayers; ++i) {
    for (int j = i + 1; j < numPlayers; ++j) {
      teamQueue.push(
          std::make_pair(std::abs(teamData.winPercentages(i, j) - 50.0),
                         std::make_pair(i, j)));
    }
  }

  // Assign players to teams
  std::vector<std::pair<int, int>> teams;
  std::vector<bool> assigned(numPlayers, false);
  while (teams.size() < numPlayers / 2) {
    std::pair<int, int> team = teamQueue.top().second;
    teamQueue.pop();

    if (!assigned[team.first] && !assigned[team.second]) {
      teams.push_back(team);
      assigned[team.first] = true;
      assigned[team.second] = true;

      // Remove all teams with any player from this team
      for (auto& q = teamQueue; !q.empty();) {
        auto& t = q.top().second;
        if (t.first == team.first || t.first == team.second ||
            t.second == team.first || t.second == team.second) {
          q.pop();
        } else {
          break;
        }
      }
    }
  }

  // Output the resulting teams
  nlohmann::json output;
  output["teams"] = teams;
  std::cout << output.dump() << std::endl;

  return 0;
}

*/
