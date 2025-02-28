#include "Point.h"
#include <iostream>
#include <fstream>

std::vector<Point> readSpherePositions(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return points;
    }

    double x, y, z;
    while (file >> x >> y >> z) {
        points.push_back({ x, y, z });
    }

    file.close();
    return points;
}
