#pragma once
#include <vector>
#include <unordered_set>
#include "Geometry.h"
#include "BoundingBox.h"

using namespace std;

class Monomer {
public:
    Monomer(const vector<Vertex>& innerVerts, const vector<Face>& faces);
    double calculateVolume(const vector<Vertex>& vertices) const;
    double calculateClippedVolume(const vector<Vertex>& vertices, const BoundingBox& box) const;
    vector<Vertex> innerVertices;

private:
    vector<Face> faces;
    Vertex centrVertex;                          

    Vertex calculateCenter(const vector<Vertex>& vertices) const; 
};
