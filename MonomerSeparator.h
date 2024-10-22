#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Geometry.h"

class MonomerSeparator {
public:
    // Розділення фейсів на мономери
    std::vector<std::vector<Face>> separateMonomers(const std::vector<Face>& faces, const std::vector<Vertex>& vertices);

private:
    // Створення графа для вершини та фейсів
    std::unordered_map<int, std::unordered_set<int>> buildGraph(const std::vector<Face>& faces);

    // Пошук компоненти зв'язності для мономера
    void findConnectedComponent(int startVertex, const std::unordered_map<int, std::unordered_set<int>>& graph,
        std::unordered_set<int>& visited, std::vector<int>& component);

    // Витягування фейсів, які належать одному компоненту
    std::vector<Face> extractFacesForComponent(const std::vector<Face>& faces, const std::vector<int>& component);
};