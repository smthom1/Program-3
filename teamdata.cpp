#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <cmath>
#include "json.hpp"

struct CompareTeam {
  bool operator()(const std::pair<double, std::pair<int, int>> &t1, const std::pair<double, std::pair<int, int>> &t2) {
    return std::abs(t1.first - 0.5) > std::abs(t2.first - 0.5);
  }
};

int main(int argc, char** argv) {
  std::priority_queue<std::pair<double, std::pair<int, int>>, std::vector<std::pair<double, std::pair<int, int>>>, CompareTeam> teamQueue;

  // Read the JSON data
  json data;
  try {
    std::cin >> data;
  } catch (const std::exception& e) {
    std::cerr << "Error reading input JSON: " << e.what() << std::endl;
    return 1;
  }

  const int numPlayers = data["metadata"]["numPlayers"];
  std::vector<std::vector<bool>> usedPlayers(numPlayers, std::vector<bool>(numPlayers, false));
  std::vector<std::pair<int, int>> teams;

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

  return 0;
}
