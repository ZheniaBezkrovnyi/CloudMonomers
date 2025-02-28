#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>

struct Point {
    double x, y, z;
};

std::vector<Point> readSpherePositions(const std::string& filename);

#endif
