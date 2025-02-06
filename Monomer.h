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
    void Destroy();
    vector<Vertex> innerVertices;
    unordered_set<Monomer*> neighborMonomers;


    bool operator==(const Monomer& other) const {
        if (innerVertices.size() == other.innerVertices.size()) {
            if (innerVertices.size() == 0 || other.innerVertices.size() == 0) return false;

            if (innerVertices.size() == 1) {
                if (innerVertices[0] == other.innerVertices[0]) {
                    return true;
                }
                return false;
            }
            else if (innerVertices.size() > 1) {
                if (innerVertices[0] == other.innerVertices[0] &&
                    innerVertices[innerVertices.size() - 1] == other.innerVertices[other.innerVertices.size() - 1]) {
                    return true;
                }
                return false;
            }

            return false;
        }
    }

private:
    vector<Face> faces;
    Vertex centrVertex;                          

    Vertex calculateCenter(const vector<Vertex>& vertices) const; 
    void calculateNeighborMonomers(const unordered_set<Monomer*>& monomers);
};
