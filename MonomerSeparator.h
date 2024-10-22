#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Geometry.h"

class MonomerSeparator {
public:
    // ��������� ����� �� ��������
    std::vector<std::vector<Face>> separateMonomers(const std::vector<Face>& faces, const std::vector<Vertex>& vertices);

private:
    // ��������� ����� ��� ������� �� �����
    std::unordered_map<int, std::unordered_set<int>> buildGraph(const std::vector<Face>& faces);

    // ����� ���������� ��'������ ��� ��������
    void findConnectedComponent(int startVertex, const std::unordered_map<int, std::unordered_set<int>>& graph,
        std::unordered_set<int>& visited, std::vector<int>& component);

    // ����������� �����, �� �������� ������ ����������
    std::vector<Face> extractFacesForComponent(const std::vector<Face>& faces, const std::vector<int>& component);
};