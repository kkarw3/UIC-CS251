// Krrish Karwal

#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue>  // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

double INF = numeric_limits<double>::max();

// A priority queue class used for dijkstra later on
class prioritize
{
  public:
    bool operator()(const pair<long long, double>& p1, const pair<long long, double>& p2) const
    {
      return p1.second > p2.second;
    }
};


void buildGraph(istream& input, graph<long long, double>& G, vector<BuildingInfo>& buildings, unordered_map<long long, Coordinates>& coords)
{
  // Parse the entire input stream into a JSON object
  json j;
  input >> j;

  // ------------------------------------------------------------
  // (1) Load all buildings into 'buildings' and add them as
  //     graph vertices. Buildings are not stored in 'coords'.
  // ------------------------------------------------------------

  for (const auto& b : j["buildings"])
  {
    // Gets the info of the current building
    long long id = b["id"];
    double lat   = b["lat"];
    double lon   = b["lon"];
    string abbr  = b["abbr"];
    string name  = b["name"];

    // Create a coordinate pair for the current building
    Coordinates c(lat, lon);

    // Adds the current building's info to the 'buildings' vector
    buildings.emplace_back(id, c, name, abbr);

    // Adds the current building's ID as a vertex to the graph
    G.addVertex(id);
  }

  // ------------------------------------------------------------
  // (2) Loads all waypoints from 'j' into 'coords' and adds each one
  //     as a  Waypoints get stored in 'coords' so that
  //     footway edges can be built later on in the function.
  // ------------------------------------------------------------

  for (const auto& w : j["waypoints"])
  {
    // Gets the info of the current waypoint
    long long id = w["id"];
    double lat = w["lat"];
    double lon = w["lon"];

    // Create a coordinate pair for the current waypoint
    Coordinates c(lat, lon);

    // Adds the waypoint to 'coords'
    coords[id] = c;

    // Adds the current waypoint's ID as a vertex to the graph
    G.addVertex(id);
  }

  // ------------------------------------------------------------
  // (3) Build edges for each footway. Footways list ordered
  //     sequences of waypoint IDs that must be connected.
  // ------------------------------------------------------------

  for (const auto& f : j["footways"])
  {
    // If there are less than 2 footways in the JSON object, skips iterating through the footways list
    if (f.size() < 2)
    {
      continue;
    }

    for (size_t i = 0; i + 1 < f.size(); ++i)
    {
      // Gets two footways from the list
      long long v1 = f[i];
      long long v2 = f[i + 1];

      // Gets the coordinates for the two footways
      Coordinates c1 = coords[v1];
      Coordinates c2 = coords[v2];

      // Compute the walking distance between the two points
      double dist = distBetween2Points(c1, c2);

      // Adds an undirected edge between the two footways
      G.addEdge(v1, v2, dist);
      G.addEdge(v2, v1, dist);
    }
  }

  // ------------------------------------------------------------
  // (4) Connect buildings to nearby waypoints.
  //     A waypoint is considered "nearby" if it is within
  //     MAX_DIST degrees of latitude/longitude.
  //
  //     First, builds a list of all the waypoint IDs only
  // ------------------------------------------------------------

  const double MAX_DIST = 0.036;

  vector<long long> waypointIDs;
  waypointIDs.reserve(coords.size());   // reserve max size

  // Identify all waypoint IDs in 'coords'
  for (auto& entry : coords)
  {
    long long id = entry.first;

    // Check if the current ID belongs to a building
    bool isBuilding = false;
    
    // If the current ID matches a building's ID, moves to the next ID
    for (auto& b : buildings)
    {
      if (b.id == id)
      {
        isBuilding = true;
        break;
      }
    }

    // Add only non-building IDs
    if (!isBuilding)
    {
      waypointIDs.push_back(id);
    }
  }

  // ------------------------------------------------------------
  // (5) For each building, add edges to all nearby waypoints.
  //     These edges allow routing from buildings onto the
  //     footway network.
  // ------------------------------------------------------------

  for (const BuildingInfo& b : buildings)
  {
    for (long long wp : waypointIDs)
    {
      Coordinates cb = b.location;   // building coordinate
      Coordinates cw = coords[wp];   // waypoint coordinate

      double d = distBetween2Points(cb, cw);

      // If the waypoint is within MAX_DIST, connects the building and waypoint
      if (d <= MAX_DIST)
      {
        G.addEdge(b.id, wp, d);
        G.addEdge(wp, b.id, d);
      }
    }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo>& buildings,
                             const string& query) {
  for (const BuildingInfo& building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo>& buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo& building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double>& G, long long start, long long target, const set<long long>& ignoreNodes)
{
  // If the start and target match each other, returns start in the form of vector<long long>
  if (start == target)
  {
    return vector<long long>{start};
  }

  // A distance map used to map vertices with their shortest distance
  unordered_map<long long, double> distanceMap;

  // A map that stores the previous vertex of a given vertex
  // Used later on to construct a path from the st
  unordered_map<long long, double> previous;

  // Sets each vertex's distance in the graph to INF (vertices are currently "unreachable") 
  for (long long v : G.getVertices())
  {
    distanceMap[v] = INF;
  }

  // Sets the distance for the starting vertex to 0.0 
  distanceMap[start] = 0.0;

  // Our worklist for the graph - Stores pairs of vertices and their distance from the starting vertex
  priority_queue<pair<long long, double>, vector<pair<long long, double>>, prioritize> worklist;

  // Adds the starting vertex to the worklist
  worklist.push({start, 0.0});

  // The part where we actually start working on the graph
  while (!worklist.empty())
  {
    // Gets the first pair at the top of the worklist and removes it
    auto [currVertex, currVertexDist] = worklist.top();
    worklist.pop();

    // If the current worklist entry's distance doesn't equal the current shortest distance, skips to the next worklist entry
    if (currVertexDist != distanceMap[currVertex])
    {
      continue;
    }

    // If the target vertex is reached, exit the loop
    if (currVertex == target)
    {
      break;
    }

    // If the current vertex is part of the ignoreNodes list, skips to the next worklist entry
    // Note: 'start' and 'target' are always allowed
    if ((ignoreNodes.count(currVertex)) && (currVertex != start) && (currVertex != target))
    {
      continue;
    }

    // Iterate over all neighbors of 'currVertex'
    for (auto& currVertexNeighbor: G.neighbors(currVertex))
    {
      // Gets the weight of the current neighbor of the vertex
      double weight;
      G.getWeight(currVertex, currVertexNeighbor, weight);

      // If the current neighbor is part of the ignoreNodes list, skips to the next worklist entry
      // Note: 'start' and 'target' are always allowed
      if ((ignoreNodes.count(currVertexNeighbor)) && (currVertexNeighbor != start) && (currVertexNeighbor != target))
      {
        continue;
      }

      // Computes the new distance for the graph
      double newDistance = distanceMap[currVertex] + weight;

      // If the new distance to currVertexNeighbor is shorter than before:
      // - Updates its distance
      // - records the previous vertex used to get to currVertexNeighbor
      // - Add it to the top of the priority queue
      if (newDistance < distanceMap[currVertexNeighbor])
      {
        distanceMap[currVertexNeighbor] = newDistance;
        previous[currVertexNeighbor] = currVertex;
        worklist.push({currVertexNeighbor, newDistance});
      }
    }
  }

  // If the target vertex was never reached, there's no available path from the start to the target
  // Thus, returns an empty vector
  if (distanceMap[target] == INF)
  {
    return vector<long long>{}; 
  }

  vector<long long> dragPath;       // The shortest path from 'start' to 'target'
  long long currentVertex = target; // The current vertex while iterating through the graph

  // Builds a path from the starting vertex to the target vertex by going backwards (i.e. from 'target' to 'start')
  while (currentVertex != start)
  {
    dragPath.push_back(currentVertex);
    currentVertex = previous[currentVertex];
  }
  dragPath.push_back(start);

  // Reverses the path so that it's in order from 'start' to 'target'
  reverse(dragPath.begin(), dragPath.end());

  return dragPath;
}

double pathLength(const graph<long long, double>& G,
                  const vector<long long>& path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long>& path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo>& buildings,
                 const graph<long long, double>& G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto& building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
