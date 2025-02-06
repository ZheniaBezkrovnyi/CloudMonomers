#pragma once
#include <string>
#include <vector>
#include "Geometry.h"
#include "Monomer.h"

class HandlerPLY {
public:
    HandlerPLY(const std::string& fileName);
    int processManuallyBounds() const;

private:
    std::string _fileName;
};