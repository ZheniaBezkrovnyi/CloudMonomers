#pragma once
#include <vector>
#include "Geometry.h"  

struct BoundingBox {
    Vertex minVertex;
    Vertex maxVertex;

    BoundingBox operator*(float scale) {
        Vertex center;
        center.x = (minVertex.x + maxVertex.x) / 2.0f;
        center.y = (minVertex.y + maxVertex.y) / 2.0f;
        center.z = (minVertex.z + maxVertex.z) / 2.0f;

        BoundingBox result;
        float width = (maxVertex.x - minVertex.x) * scale;
        float height = (maxVertex.y - minVertex.y) * scale;
        float depth = (maxVertex.z - minVertex.z) * scale;

        result.minVertex.x = center.x - width / 2;
        result.minVertex.y = center.y - height / 2;
        result.minVertex.z = center.z - depth / 2;

        result.maxVertex.x = center.x + width / 2;
        result.maxVertex.y = center.y + height / 2;
        result.maxVertex.z = center.z + depth / 2;

        return result;
    }
};

BoundingBox findBoundingBox(const std::vector<Vertex>& vertices);

double findVolume(const BoundingBox& boundingBox);

bool insideBox(const Vertex& vertex, const BoundingBox& box);
