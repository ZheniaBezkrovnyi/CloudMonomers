#pragma once
#include <vector>
#include "Geometry.h"

class MonomerSeparator {
public:
    std::vector<std::vector<Face>> separateMonomers(const std::vector<Face>& faces);
};
