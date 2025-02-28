#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>

struct Sphere {
    int id;
    double x, y, z, radius;
    double accuracy;
    std::vector<int> neighbors;

    Sphere(int id, double x, double y, double z, double radius);

    bool isTouching(const Sphere& other) const;
};

#endif 
