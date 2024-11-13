#include "MonomerSeparator.h"
#include <unordered_set>
#include <iostream>

using namespace std;


void MonomerSeparator::addNeighbors(vector<Vertex>& vertices, const Face& face, int faceIndex) {
    int vCount = vertices.size();
    if (face.v1 >= vCount || face.v2 >= vCount || face.v3 >= vCount)
        return;

    vertices[face.v1].neighbors.insert(face.v2);
    vertices[face.v1].neighbors.insert(face.v3);
    vertices[face.v2].neighbors.insert(face.v1);
    vertices[face.v2].neighbors.insert(face.v3);
    vertices[face.v3].neighbors.insert(face.v1);
    vertices[face.v3].neighbors.insert(face.v2);

    vertices[face.v1].faceIndices.insert(faceIndex);
    vertices[face.v2].faceIndices.insert(faceIndex);
    vertices[face.v3].faceIndices.insert(faceIndex);
}
void MonomerSeparator::dfs(int vertexIndex, vector<bool>& visited, const vector<Vertex>& vertices, const vector<Face>& faces, unordered_set<int>& component, set<int>& faceIndices) {
    
    if (visited.size() <= vertexIndex) return; 

    visited[vertexIndex] = true;
    component.insert(vertexIndex);
    for (int neighbor : vertices[vertexIndex].neighbors) {
        if (visited.size() > neighbor && !visited[neighbor]) {
            dfs(neighbor, visited, vertices, faces, component, faceIndices);
        }
    }

    for (int faceIndex : vertices[vertexIndex].faceIndices) {
        const Face& face = faces[faceIndex];
        if (component.count(face.v1) && component.count(face.v2) && component.count(face.v3)) {
            if (faceIndices.find(faceIndex) == faceIndices.end()) {
                faceIndices.insert(faceIndex);
            }
        }
    }
}