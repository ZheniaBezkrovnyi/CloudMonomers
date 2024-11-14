#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <cstdint>
#include <unordered_set>

using namespace std;

struct Vertex {
    int index;

    float x, y, z;
    set<int> neighbors;
    set<int> faceIndices;

    Vertex operator-(const Vertex& other) const {
        Vertex result;
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.z = this->z - other.z;

        return result;
    }
};

struct Face {
    int v1, v2, v3;
};