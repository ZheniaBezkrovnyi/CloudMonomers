#pragma once
#include <vector>
#include "Geometry.h"

class MonomerSeparator {
public:
    void addNeighbors(std::vector<Vertex>& vertices, const Face& face, int faceIndex);

    void dfs(int vertexIndex, std::vector<bool>& visited, const std::vector<Vertex>& vertices, const vector<Face>& faces, std::unordered_set<int>& component, set<int>& faceIndices);
};