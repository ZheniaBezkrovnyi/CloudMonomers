#include "Scene.h"
#include <iostream>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <numeric>

void Scene::addSphere(double x, double y, double z, double radius) {
    Sphere newSphere(next_id++, x, y, z, radius);

    for (auto& sphere : spheres) {
        if (newSphere.isTouching(sphere)) {
            if (std::find(newSphere.neighbors.begin(), newSphere.neighbors.end(), sphere.id) == newSphere.neighbors.end()) {
                newSphere.neighbors.push_back(sphere.id);
            }
            if (std::find(sphere.neighbors.begin(), sphere.neighbors.end(), newSphere.id) == sphere.neighbors.end()) {
                sphere.neighbors.push_back(newSphere.id);
            }
        }
    }

    spheres.push_back(newSphere);
}

void Scene::removeSphere(int id) {
    for (size_t i = 0; i < spheres.size(); ++i) {
        if (spheres[i].id == id) {
            for (auto& sphere : spheres) {
                sphere.neighbors.erase(
                    std::remove(sphere.neighbors.begin(), sphere.neighbors.end(), id),
                    sphere.neighbors.end()
                );
            }
            spheres.erase(spheres.begin() + i);
            break;
        }
    }
}

void Scene::removeRandomSpheres(int count) {
    if (spheres.size() < count) count = spheres.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, spheres.size() - 1);

    std::unordered_set<int> removedIds;

    while (removedIds.size() < count) {
        int index = dist(gen);
        removedIds.insert(spheres[index].id);
    }

    std::vector<Sphere> newSpheres;
    for (const auto& sphere : spheres) {
        if (!removedIds.count(sphere.id)) {
            newSpheres.push_back(sphere);
        }
    }

    for (auto& sphere : newSpheres) {
        std::vector<int> newNeighbors;
        for (int neighbor : sphere.neighbors) {
            if (!removedIds.count(neighbor)) {
                newNeighbors.push_back(neighbor);
            }
        }
        sphere.neighbors = std::move(newNeighbors);
    }

    spheres = std::move(newSpheres);
}

std::vector<std::vector<int>> Scene::findClusters() const {
    std::vector<std::vector<int>> clusters;
    std::unordered_set<int> visited;

    std::unordered_map<int, int> idToIndex;
    for (size_t i = 0; i < spheres.size(); ++i) {
        idToIndex[spheres[i].id] = i;
    }

    for (const auto& sphere : spheres) {
        if (visited.count(sphere.id)) continue;

        std::vector<int> cluster;
        std::queue<int> q;
        q.push(sphere.id);
        visited.insert(sphere.id);

        while (!q.empty()) {
            int current_id = q.front();
            q.pop();
            cluster.push_back(current_id);

            if (idToIndex.find(current_id) == idToIndex.end()) continue;
            const auto& sphereRef = spheres.at(idToIndex.at(current_id));

            for (const auto& neighbor_id : sphereRef.neighbors) {
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
        if (cluster.size() < x) {
            toRemove.insert(cluster.begin(), cluster.end());
        }
    }

    std::vector<Sphere> newSpheres;
    for (const auto& sphere : spheres) {
        if (!toRemove.count(sphere.id)) {
            newSpheres.push_back(sphere);
        }
    }

    for (auto& sphere : newSpheres) {
        std::vector<int> newNeighbors;
        for (int neighbor : sphere.neighbors) {
            if (!toRemove.count(neighbor)) {
                newNeighbors.push_back(neighbor);
            }
        }
        sphere.neighbors = std::move(newNeighbors);
    }

    spheres = std::move(newSpheres);
}

void Scene::printScene() const {
    std::cout << "Scene contains " << spheres.size() << " spheres.\n";
}

double Scene::calculateAverageCoordinationNumber() const {
    if (spheres.empty()) return 0.0;

    int totalNeighbors = 0;
    for (const auto& sphere : spheres) {
        totalNeighbors += sphere.neighbors.size();
    }

    return static_cast<double>(totalNeighbors) / spheres.size();
}

std::unordered_map<int, double> Scene::calculateClusterCoordinationNumbers() const {
    std::unordered_map<int, double> clusterCoordNumbers;
    auto clusters = findClusters();

    for (const auto& cluster : clusters) {
        int clusterSize = cluster.size();
        int totalNeighbors = 0;

        for (int sphereID : cluster) {
            auto it = std::find_if(spheres.cbegin(), spheres.cend(),
                [&](const Sphere& s) { return s.id == sphereID; });

            if (it != spheres.cend()) { 
                totalNeighbors += it->neighbors.size();
            }
        }

        if (!cluster.empty()) {
            clusterCoordNumbers[clusterSize] = static_cast<double>(totalNeighbors) / cluster.size();
        }
    }

    return clusterCoordNumbers;
}
