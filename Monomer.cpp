#include "Monomer.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <unordered_set>

using namespace std;

Monomer::Monomer(const vector<Vertex>& innerVerts, const vector<Face>& faces) : innerVertices(innerVerts), faces(faces) {
    if (innerVerts.size() < 3) {
        throw runtime_error("Not enough vertices");
    }
    centrVertex = calculateCenter(innerVerts);
}

double Monomer::calculateVolume(const std::vector<Vertex>& vertices) const {
    double volume = 0.0;

    for (const auto& face : faces) {
        Vertex v0 = vertices[face.v1];
        Vertex v1 = vertices[face.v2];
        Vertex v2 = vertices[face.v3];

        Vertex ab = v1 - v0;
        Vertex ac = v2 - v0;

        Vertex crossProduct;
        crossProduct.x = ab.y * ac.z - ab.z * ac.y;
        crossProduct.y = ab.z * ac.x - ab.x * ac.z;
        crossProduct.z = ab.x * ac.y - ab.y * ac.x;

        double dotProduct = v0.x * crossProduct.x + v0.y * crossProduct.y + v0.z * crossProduct.z;

        volume += dotProduct / 6.0;

    }
    return volume;
}



double Monomer::calculateClippedVolume(const vector<Vertex>& vertices, const BoundingBox& box) const {
    int insideCount = 0;

    for (const auto& vertex : innerVertices) {
        if (insideBox(vertex, box))
            insideCount++;
    }

    return (float)insideCount / innerVertices.size() * calculateVolume(vertices);
}
void Monomer::Destroy() {
    for (Monomer* neighbor : neighborMonomers) {
        neighbor->neighborMonomers.erase(this);
    }
}




Vertex Monomer::calculateCenter(const vector<Vertex>& vertices) const {
    Vertex center = { 0.0, 0.0, 0.0 };
    for (const auto& vertex : vertices) {
        center.x += vertex.x;
        center.y += vertex.y;
        center.z += vertex.z;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();

    return center;
}


void Monomer::calculateNeighborMonomers(const unordered_set<Monomer*>& monomers) {
    for (Monomer* monomer : monomers) {
        if (this == monomer) continue;

        for (const auto& vertex : innerVertices) {
            if (find(monomer->innerVertices.begin(), monomer->innerVertices.end(), vertex) != monomer->innerVertices.end()) {
                neighborMonomers.insert(monomer); 
                break;
            }
        }
    }
}