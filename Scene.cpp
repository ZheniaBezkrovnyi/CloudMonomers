#include "Scene.h"
#include <iostream>
#include <unordered_set>
#include <queue>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>

std::tuple<int, int, int> Scene::getCell(double x, double y, double z) const {
    return std::make_tuple(
        static_cast<int>(x / cellSize),
        static_cast<int>(y / cellSize),
        static_cast<int>(z / cellSize)
    );
}

double Scene::computeOptimalCellSize(double avgRadius, int sphereCount) const {
    const double baseMultiplier = 2.5;
    const double logFactor = std::log10(static_cast<double>(sphereCount) + 10.0);
    return avgRadius * baseMultiplier * logFactor;
}

void Scene::initCellSizeFromSpheres(double radius, int size) {
    cellSize = computeOptimalCellSize(radius, size);
    std::cout << "cellSize " << cellSize << " \n";
}

void Scene::addSphere(double x, double y, double z, double radius) {
    Sphere newSphere(next_id++, x, y, z, radius);

    auto cell = getCell(x, y, z);
    std::vector<std::tuple<int, int, int>> nearbyCells;

    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dz = -1; dz <= 1; ++dz)
                nearbyCells.push_back(std::make_tuple(
                    std::get<0>(cell) + dx,
                    std::get<1>(cell) + dy,
                    std::get<2>(cell) + dz
                ));

    for (const auto& neighborCell : nearbyCells) {
        auto it = spatialHash.find(neighborCell);
        if (it == spatialHash.end()) continue;

        for (int neighborID : it->second) {
            Sphere& sphere = spheres.at(neighborID);
            if (newSphere.isTouching(sphere)) {
                if (std::find(newSphere.neighbors.begin(), newSphere.neighbors.end(), sphere.id) == newSphere.neighbors.end())
                    newSphere.neighbors.push_back(sphere.id);
                if (std::find(sphere.neighbors.begin(), sphere.neighbors.end(), newSphere.id) == sphere.neighbors.end())
                    sphere.neighbors.push_back(newSphere.id);
            }
        }
    }

    spheres[newSphere.id] = newSphere;
    spatialHash[cell].push_back(newSphere.id);
}

void Scene::removeSphere(int id) {
    if (!spheres.count(id)) return;

    for (auto& pair : spheres) {
        Sphere& sphere = pair.second;
        sphere.neighbors.erase(
            std::remove(sphere.neighbors.begin(), sphere.neighbors.end(), id),
            sphere.neighbors.end()
        );
    }

    spheres.erase(id);
}

void Scene::removeRandomSpheres(int count) {
    if (spheres.size() < static_cast<size_t>(count)) count = static_cast<int>(spheres.size());

    std::vector<int> ids;
    for (const auto& pair : spheres)
        ids.push_back(pair.first);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(ids.begin(), ids.end(), gen);

    std::unordered_set<int> toRemove(ids.begin(), ids.begin() + count);

    for (int id : toRemove)
        spheres.erase(id);

    for (auto& pair : spheres) {
        Sphere& sphere = pair.second;
        std::vector<int> newNeighbors;
        for (int neighbor : sphere.neighbors) {
            if (!toRemove.count(neighbor))
                newNeighbors.push_back(neighbor);
        }
        sphere.neighbors = std::move(newNeighbors);
    }
}

std::vector<std::vector<int>> Scene::findClusters() const {
    std::vector<std::vector<int>> clusters;
    std::unordered_set<int> visited;

    for (const auto& pair : spheres) {
        int id = pair.first;
        const Sphere& sphere = pair.second;
        if (visited.count(id)) continue;

        std::vector<int> cluster;
        std::queue<int> q;
        q.push(id);
        visited.insert(id);

        while (!q.empty()) {
            int current_id = q.front();
            q.pop();
            cluster.push_back(current_id);

            const auto& neighbors = spheres.at(current_id).neighbors;
            for (int neighbor_id : neighbors) {
                if (!visited.count(neighbor_id)) {
                    visited.insert(neighbor_id);
                    q.push(neighbor_id);
                }
            }
        }

        clusters.push_back(cluster);
    }

    return clusters;
}

void Scene::processClusters(int x) {
    auto clusters = findClusters();
    std::unordered_set<int> toRemove;

    for (const auto& cluster : clusters) {
        if (static_cast<int>(cluster.size()) < x) {
            toRemove.insert(cluster.begin(), cluster.end());
        }
    }

    for (int id : toRemove)
        spheres.erase(id);

    for (auto& pair : spheres) {
        Sphere& sphere = pair.second;
        std::vector<int> newNeighbors;
        for (int neighbor : sphere.neighbors) {
            if (!toRemove.count(neighbor)) {
                newNeighbors.push_back(neighbor);
            }
        }
        sphere.neighbors = std::move(newNeighbors);
    }
}

void Scene::printScene() const {
    std::cout << "Scene contains " << spheres.size() << " spheres.\n";
}

double Scene::calculateAverageCoordinationNumber() const {
    if (spheres.empty()) return 0.0;
    int totalNeighbors = 0;
    for (const auto& pair : spheres) {
        totalNeighbors += static_cast<int>(pair.second.neighbors.size());
    }
    return static_cast<double>(totalNeighbors) / spheres.size();
}

std::unordered_map<int, double> Scene::calculateClusterCoordinationNumbers() const {
    std::unordered_map<int, double> clusterCoordNumbers;
    auto clusters = findClusters();

    for (const auto& cluster : clusters) {
        int totalNeighbors = 0;
        for (int sphereID : cluster) {
            const auto& sphere = spheres.at(sphereID);
            totalNeighbors += static_cast<int>(sphere.neighbors.size());
        }
        if (!cluster.empty())
            clusterCoordNumbers[static_cast<int>(cluster.size())] = static_cast<double>(totalNeighbors) / cluster.size();
    }

    return clusterCoordNumbers;
}