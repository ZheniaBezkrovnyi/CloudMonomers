#pragma once
#include <vector>
#include <unordered_set>
#include "Geometry.h"
#include "BoundingBox.h"

using namespace std;

class Monomer {
public:
    Monomer(const vector<Vertex>& vertices, const vector<Face>& faces);
    double calculateVolume() const;  
    double calculateClippedVolume(const BoundingBox& box) const;

private:
    vector<Vertex> vertices; 
    vector<Face> faces;
    Vertex centrVertex;                          

    Vertex calculateCenter(const vector<Vertex>& vertices) const; 
};
