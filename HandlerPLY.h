#pragma once
#include <string>
#include <vector>
#include "Geometry.h"

class HandlerPLY {
public:
    HandlerPLY(const std::string& fileName);  
    void process() const;                  

private:
    std::string _fileName;      
};
