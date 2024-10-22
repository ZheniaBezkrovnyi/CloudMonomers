#include "MonomerSeparator.h"
#include "MonomerSeparator.h"
#include <stack>
#include <iostream>

using namespace std;

// ����� ��� ��������� ����� �� ����� ��������
vector<vector<Face>> MonomerSeparator::separateMonomers(const vector<Face>& faces, const vector<Vertex>& vertices) {
    // �������� ����� � ������ � �����
    unordered_map<int, unordered_set<int>> graph = buildGraph(faces);

    // ³��������� �������� ������
    unordered_set<int> visited;
    vector<vector<Face>> monomers;

    // ������ ��� ������
    for (int i = 0; i < vertices.size(); ++i) {
        // ���� ������� �� �� �������, ������ ��������� ��'������
        if (visited.find(i) == visited.end()) {
            vector<int> component;
            findConnectedComponent(i, graph, visited, component);

            // �������� ����� ��� ������ ����������
            vector<Face> monomerFaces = extractFacesForComponent(faces, component);
            if (!monomerFaces.empty()) {
                monomers.push_back(monomerFaces);
            }
        }
    }

    return monomers;
}

// ����� ��� �������� ����� � ������ � �����
unordered_map<int, unordered_set<int>> MonomerSeparator::buildGraph(const vector<Face>& faces) {
    unordered_map<int, unordered_set<int>> graph;

    // ������ ����� �� ��������� ������� �����
    for (const Face& face : faces) {
        graph[face.v1].insert(face.v2);
        graph[face.v1].insert(face.v3);
        graph[face.v2].insert(face.v1);
        graph[face.v2].insert(face.v3);
        graph[face.v3].insert(face.v1);
        graph[face.v3].insert(face.v2);
    }

    return graph;
}

// ����� ���������� ��'������ ��� �������� � ������������� DFS
void MonomerSeparator::findConnectedComponent(int startVertex, const unordered_map<int, unordered_set<int>>& graph,
    unordered_set<int>& visited, vector<int>& component) {
    stack<int> toVisit;
    toVisit.push(startVertex);

    while (!toVisit.empty()) {
        int vertex = toVisit.top();
        toVisit.pop();

        if (visited.find(vertex) == visited.end()) {
            visited.insert(vertex);
            component.push_back(vertex);

            // ������ ����� ������� �� ����� ��� ����������
            for (int neighbor : graph.at(vertex)) {
                if (visited.find(neighbor) == visited.end()) {
                    toVisit.push(neighbor);
                }
            }
        }
    }
}

// ����� ��� ����������� �����, �� �������� ������ ����������
vector<Face> MonomerSeparator::extractFacesForComponent(const vector<Face>& faces, const vector<int>& component) {
    unordered_set<int> componentSet(component.begin(), component.end());
    vector<Face> monomerFaces;

    // ����������, �� �� ������� ����� �������� ����������
    for (const Face& face : faces) {
        if (componentSet.find(face.v1) != componentSet.end() &&
            componentSet.find(face.v2) != componentSet.end() &&
            componentSet.find(face.v3) != componentSet.end()) {
            monomerFaces.push_back(face);
        }
    }

    return monomerFaces;
}
