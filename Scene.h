#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include <vector>
#include <algorithm>

class Scene {
private:
    std::vector<Sphere> spheres;
    int next_id = 0;

public:
    void addSphere(double x, double y, double z, double radius);
    void removeSphere(int id);
    void printScene() const;

    void removeRandomSpheres(int count);
    std::vector<std::vector<int>> findClusters();
    void processClusters(int x);
};

#endif 
