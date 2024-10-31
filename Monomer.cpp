#include "Monomer.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <unordered_set>

using namespace std;

Monomer::Monomer(const vector<Vertex>& verts) : vertices(verts) {
    if (verts.size() < 3) {
        throw runtime_error("Not enough vertices");
    }
    centrVertex = calculateCenter(verts);
}

double Monomer::calculateVolume() const {
    double volume = 0.0;
    Vertex center = calculateCenter(vertices);  

    for (size_t i = 0; i < vertices.size() - 2; ++i) {
        Vertex v0 = vertices[0];
        Vertex v1 = vertices[i + 1];
        Vertex v2 = vertices[i + 2];

        Vertex crossProduct = {
            (v1.y - v0.y) * (v2.z - v0.z) - (v1.z - v0.z) * (v2.y - v0.y),
            (v1.z - v0.z) * (v2.x - v0.x) - (v1.x - v0.x) * (v2.z - v0.z),
            (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x)
        };

        double dotProduct = (v0.x - center.x) * crossProduct.x +
            (v0.y - center.y) * crossProduct.y +
            (v0.z - center.z) * crossProduct.z;

        volume += dotProduct;
    }

    return abs(volume / 6.0); 
}

double Monomer::calculateClippedVolume(const BoundingBox& box) const {
    int insideCount = 0;

    for (const auto& vertex : vertices) {
        if (insideBox(vertex, box))
            insideCount++;
    }

    return insideCount / vertices.size() * calculateVolume();
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
