#pragma once
#include <vector>
#include <unordered_set>
#include "Geometry.h"

using namespace std;

class Monomer {
public:
    Monomer(const vector<Vertex>& verts);           
    Monomer(const vector<Face>& faces, const vector<Vertex>& vertices); 
    double calculateVolume() const;        
    vector<Vertex> extractVerticesFromFaces(const vector<Face>& faces, const vector<Vertex>& vertices); 

private:
    vector<Vertex> vertices;                      
    Vertex centrVertex;                          

    Vertex calculateCenter(const vector<Vertex>& vertices) const; 
};
