#pragma once
#include <vector>
#include "Geometry.h"

class MonomerSeparator {
public:
    void addNeighbors(std::vector<Vertex>& vertices, const Face& face);

    void dfs(int v, std::vector<bool>& visited, const std::vector<Vertex>& vertices, std::unordered_set<int>& component);
};