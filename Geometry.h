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
};

struct Face {
    int v1, v2, v3;
};