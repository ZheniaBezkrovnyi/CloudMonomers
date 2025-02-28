#include <iostream>
#include <vector>
#include <cmath>

#include "Sphere.h"

Sphere::Sphere(int id, double x, double y, double z, double radius)
    : id(id), x(x), y(y), z(z), radius(radius), accuracy(radius * 0.012) {}

bool Sphere::isTouching(const Sphere& other) const {
    double distance = std::sqrt(
        (x - other.x) * (x - other.x) +
        (y - other.y) * (y - other.y) +
        (z - other.z) * (z - other.z)
    );
    return distance <= (radius + other.radius) + accuracy;
}