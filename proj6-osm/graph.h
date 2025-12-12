// Krrish Karwal

#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph
{
 private:
  // A map used to store a graph's vertices. 
  // Each vertex has its own map that contains any adjacent vertices along with their corresponding weight.
  unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjacencyList;

  // Keeps track of how many edges exist in the graph.
  size_t edgeCount;

 public:
  /// Default constructor
  graph()
  {
    edgeCount = 0;
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v)
  {
    // Inserts v into the adjacency list
    auto iterator = adjacencyList.emplace(v, unordered_map<VertexT, WeightT>());

    // If iterator's second value equals true (insertion was successful), returns true
    // Else, returns false (insertion failed)
    return iterator.second;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {
    // Looks for both vertices in the adjacency list
    auto iteratorFrom = adjacencyList.find(from);
    auto iteratorTo = adjacencyList.find(to);

    // If either of the two vertices weren't found, returns false
    if ((iteratorFrom == adjacencyList.end()) ||(iteratorTo == adjacencyList.end()))
    {
      return false;
    }

    // Used later on to check if the edge count needs to be incremented if this is a new edge
    bool edgeExists = iteratorFrom->second.count(to);

    // Adds the edge and its corresponding weight to 
    iteratorFrom->second[to] = weight;

    // If the edge being inserted is a new edge, increments the edge count by one
    if (!edgeExists)
    {
      edgeCount++;
    }

    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
    // Looks for 'from' in the adjacency list
    auto iteratorFrom = adjacencyList.find(from);

    // Returns false if 'from' was not found
    if (iteratorFrom == adjacencyList.end())
    {
      return false;
    }

    // Looks for the neighbor edge between from and to
    auto edge = iteratorFrom->second.find(to);

    // Returns false if the edge was not found
    if (edge == iteratorFrom->second.end())
    {
      return false;
    }

    // Assigns weight with the edge's weight
    weight = edge->second;
    
    return true;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT> S;

    // Finds 'v' in the adjacency list
    auto iteratorV = adjacencyList.find(v);

    // Returns false if 'v' was not found
    if (iteratorV == adjacencyList.end())
    {
      return S;
    }

    // Inserts all of v's neighbor vertices into set 'S'
    for (const auto& pair : iteratorV->second)
    {
      S.insert(pair.first);
    }

    // Returns the set of neighbors of 'v'
    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const
  {
    vector<VertexT> vertices;

    // Gets all of the vertices from the adjacency list
    for (const auto& pair : adjacencyList)
    {
      vertices.push_back(pair.first);
    }

    // Returns the vector of vertices
    return vertices;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const
  {
    return adjacencyList.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const
  {
    return edgeCount;
  }
};
