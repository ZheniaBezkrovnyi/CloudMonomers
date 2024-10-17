#pragma once
#include <cstdint>

struct Vertex {
    float x, y, z;
};

struct Face {
    int32_t v1, v2, v3;
};