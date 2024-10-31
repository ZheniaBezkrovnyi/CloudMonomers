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
    std::set<int> neighbors;
};

struct Face {
    int32_t v1, v2, v3;
};