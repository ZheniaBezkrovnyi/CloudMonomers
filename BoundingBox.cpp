#include "BoundingBox.h"
#include <limits>

BoundingBox findBoundingBox(const std::vector<Vertex>& vertices) {
    BoundingBox box;
    box.minVertex = { 0, std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), {} };
    box.maxVertex = { 0, -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), {} };

    for (const auto& vertex : vertices) {
        if (vertex.x < box.minVertex.x) box.minVertex.x = vertex.x;
        if (vertex.y < box.minVertex.y) box.minVertex.y = vertex.y;
        if (vertex.z < box.minVertex.z) box.minVertex.z = vertex.z;
        if (vertex.x > box.maxVertex.x) box.maxVertex.x = vertex.x;
        if (vertex.y > box.maxVertex.y) box.maxVertex.y = vertex.y;
        if (vertex.z > box.maxVertex.z) box.maxVertex.z = vertex.z;
    }

    return box;
}

double findVolume(const BoundingBox& boundingBox) {
    double x = boundingBox.maxVertex.x - boundingBox.minVertex.x;
    double y = boundingBox.maxVertex.y - boundingBox.minVertex.y;
    double z = boundingBox.maxVertex.z - boundingBox.minVertex.z;

    return x * y * z;
}

bool insideBox(const Vertex& vertex, const BoundingBox& box) {
    if (vertex.x >= box.minVertex.x && vertex.x <= box.maxVertex.x &&
        vertex.y >= box.minVertex.y && vertex.y <= box.maxVertex.y &&
        vertex.z >= box.minVertex.z && vertex.z <= box.maxVertex.z) {
        return true;
    }
    return false;
}
