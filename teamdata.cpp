#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <cmath>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct CompareTeam {
  bool operator()(const pair<double, pair<int, int>> &t1, const pair<double, pair<int, int>> &t2) {
    return abs(t1.first - 0.5) > abs(t2.first - 0.5);
  }
};

int main(int argc, char** argv) {
  priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, CompareTeam> teamQueue;

  // Read the JSON data
  json data;
  cin >> data;

  int numPlayers = data["metadata"]["numPlayers"];
  vector<vector<bool>> usedPlayers(numPlayers, vector<bool>(numPlayers, false));
  vector<pair<int, int>> teams;

  // Load the data into the priority queue
  auto teamStats = data["teamStats"];
  for (auto &stat : teamStats) {
    int playerOne = stat["playerOne"];
    int playerTwo = stat["playerTwo"];
    double winPercentage = stat["winPercentage"];
    if (!usedPlayers[playerOne][playerTwo]) {
      teamQueue.push(make_pair(winPercentage, make_pair(playerOne, playerTwo)));
      usedPlayers[playerOne][playerTwo] = true;
      usedPlayers[playerTwo][playerOne] = true;
    }
  }

  // Create the teams
  while (teams.size() < numPlayers / 2) {
    auto team = teamQueue.top();
    teamQueue.pop();

    int playerOne = team.second.first;
    int playerTwo = team.second.second;
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
  for (auto &team : teams) {
    output["teams"].push_back({team.first, team.second});
  }
  cout << output << endl;

  return 0;
}
