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

double Monomer::calculateVolume(const vector<Vertex>& vertices) const {
    double volume = 0.0;
    for (const Face& face : faces) {
        Vertex v0 = vertices[face.v1];
        Vertex v1 = vertices[face.v2];
        Vertex v2 = vertices[face.v3];

        //cout << face.v1 << "   " << face.v2 << "   " << face.v3 << endl;
        /*cout << "_verticles.Add(new Vector3(" << v0.x << "f, " << v0.y << "f, " << v0.z << "f));" << endl;
        cout << "_verticles.Add(new Vector3(" << v1.x << "f, " << v1.y << "f, " << v1.z << "f));" << endl;
        cout << "_verticles.Add(new Vector3(" << v2.x << "f, " << v2.y << "f, " << v2.z << "f));" << endl;*/

        Vertex normal = {
            (v1.y - v0.y) * (v2.z - v0.z) - (v1.z - v0.z) * (v2.y - v0.y),
            (v1.z - v0.z) * (v2.x - v0.x) - (v1.x - v0.x) * (v2.z - v0.z),
            (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x)
        };

        Vertex centroid = {
            (v0.x + v1.x + v2.x) / 3,
            (v0.y + v1.y + v2.y) / 3,
            (v0.z + v1.z + v2.z) / 3
        };

        Vertex diff = {
            centroid.x - centrVertex.x,
            centroid.y - centrVertex.y,
            centroid.z - centrVertex.z
        };

        double contribution = (diff.x * normal.x + diff.y * normal.y + diff.z * normal.z) / 6.0;
        volume += contribution;
    }       //cout << "//------------" << endl;
    return abs(volume);
}

double Monomer::calculateClippedVolume(const vector<Vertex>& vertices, const BoundingBox& box) const {
    int insideCount = 0;

    for (const auto& vertex : innerVertices) {
        if (insideBox(vertex, box))
            insideCount++;
    }
    //cout << (float)insideCount / vertices.size() * calculateVolume(vertices) << "volume" << endl;
    return (float)insideCount / vertices.size() * calculateVolume(vertices);
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
