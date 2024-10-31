#include "MonomerSeparator.h"
#include <unordered_set>
#include <iostream>

using namespace std;


void MonomerSeparator::addNeighbors(std::vector<Vertex>& vertices, const Face& face) {
    vertices[face.v1].neighbors.insert(face.v2);
    vertices[face.v1].neighbors.insert(face.v3);
    vertices[face.v2].neighbors.insert(face.v1);
    vertices[face.v2].neighbors.insert(face.v3);
    vertices[face.v3].neighbors.insert(face.v1);
    vertices[face.v3].neighbors.insert(face.v2);
}
void MonomerSeparator::dfs(int vertexIndex, std::vector<bool>& visited, const std::vector<Vertex>& vertices, std::unordered_set<int>& component) {
    visited[vertexIndex] = true;
    component.insert(vertexIndex);
    for (int neighbor : vertices[vertexIndex].neighbors) {
        if (!visited[neighbor]) {
            dfs(neighbor, visited, vertices, component);
        }
    }
}