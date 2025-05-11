#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include <unordered_map>
#include <tuple>
#include <vector>

struct TupleHash {
    std::size_t operator()(const std::tuple<int, int, int>& t) const {
        int x = std::get<0>(t);
        int y = std::get<1>(t);
        int z = std::get<2>(t);

        std::size_t h1 = std::hash<int>()(x);
        std::size_t h2 = std::hash<int>()(y);
        std::size_t h3 = std::hash<int>()(z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};


class Scene {
private:
    std::unordered_map<int, Sphere> spheres;
    std::unordered_map<std::tuple<int, int, int>, std::vector<int>, TupleHash> spatialHash;
    int next_id = 0;
    double cellSize = 1.0;

    std::tuple<int, int, int> getCell(double x, double y, double z) const;

public:
    void addSphere(double x, double y, double z, double radius);
    void removeSphere(int id);
    void printScene() const;

    void removeRandomSpheres(int count);
    std::vector<std::vector<int>> findClusters() const;
    void processClusters(int x);

    double calculateAverageCoordinationNumber() const;
    std::unordered_map<int, double> calculateClusterCoordinationNumbers() const;

    void initCellSizeFromSpheres(double radius, int size);
    double computeOptimalCellSize(double avgRadius, int sphereCount) const;
};

#endif