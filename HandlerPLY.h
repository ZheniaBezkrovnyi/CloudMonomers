#pragma once
#include <string>
#include <vector>
#include "Geometry.h"
#include "Monomer.h"

class HandlerPLY {
public:
    HandlerPLY(const std::string& fileName);
    void processAutoBounds() const;
    void processManuallyBounds(const float& procentBoundingBox) const;
    std::vector<Monomer> createMonomers(
        const std::vector<std::unordered_set<int>>& clusters, 
        const std::vector<Vertex>& globalVertices) const;

private:
    std::string _fileName;
};