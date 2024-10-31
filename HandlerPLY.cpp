#include "HandlerPLY.h"
#include "PLY.h"
#include <iostream>
#include <unordered_set> 
#include "MonomerSeparator.h"
#include <fstream>
#include "BoundingBox.h"
#include "HandlerString.h"

using namespace std;

HandlerPLY::HandlerPLY(const string& fileName) : _fileName(fileName) {}

void HandlerPLY::processAutoBounds() const {
    try {
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();


        if (vertices.empty() || faces.empty()) {
            cerr << "No vertices or faces found in PLY file." << endl;
            return;
        }

        MonomerSeparator separator;

        for (const auto& face : faces) {
            separator.addNeighbors(vertices, face);
        }

        vector<bool> visited(vertices.size(), false);
        vector<unordered_set<int>> components;
        for (int i = 0; i < vertices.size(); ++i) {
            if (!visited[i]) {
                unordered_set<int> component;
                separator.dfs(i, visited, vertices, component);

                if (!component.empty()) {
                    components.push_back(component);
                }
            }
        }

        vector<Monomer> monomers = createMonomers(components, vertices);
        ofstream outFile("Monomers.txt");
        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }

        for (size_t i = 0; i < monomers.size(); ++i) {
            outFile << "Monomer Index: " << i << "  volume: " << monomers[i].calculateVolume() << endl;
        }
        outFile.close();




        double sumVolumeMonomers = 0.0;
        double sumVolumeCommon = 0.0;
        for (size_t i = 0; i < monomers.size(); ++i) {
            sumVolumeMonomers += monomers[i].calculateVolume();
        }
        BoundingBox bB = findBoundingBox(vertices);
        sumVolumeCommon = findVolume(bB);

        cout << sumVolumeMonomers / sumVolumeCommon;

        /*for (size_t i = 0; i < monomers.size(); ++i) {
            cout << "Monomer Index: " << i << "  volume: " << monomers[i].calculateVolume() << endl;
        }*/
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
void HandlerPLY::processManuallyBounds(const float& procentBoundingBox) const {
    try {
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();


        if (vertices.empty() || faces.empty()) {
            cerr << "No vertices or faces found in PLY file." << endl;
            return;
        }

        MonomerSeparator separator;

        for (const auto& face : faces) {
            separator.addNeighbors(vertices, face);
        }

        vector<bool> visited(vertices.size(), false);
        vector<unordered_set<int>> components;
        for (int i = 0; i < vertices.size(); ++i) {
            if (!visited[i]) {
                unordered_set<int> component;
                separator.dfs(i, visited, vertices, component);

                if (!component.empty()) {
                    components.push_back(component);
                }
            }
        }

        vector<Monomer> monomers = createMonomers(components, vertices);

        double sumVolumeMonomers = 0.0;
        double sumVolumeCommon = 0.0;

        BoundingBox bB = findBoundingBox(vertices) * procentBoundingBox;
        sumVolumeCommon = findVolume(bB);

        for (size_t i = 0; i < monomers.size(); ++i) {
            sumVolumeMonomers += monomers[i].calculateClippedVolume(bB);
        }

        cout << getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + ".txt" << endl;
        ofstream outFile(getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox*100)) + ".txt");
        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }
        outFile << (sumVolumeMonomers / sumVolumeCommon * 100) << " %" << endl;
        for (size_t i = 0; i < monomers.size(); ++i) {
            outFile << "Monomer Index: " << i << "  volume: " << monomers[i].calculateClippedVolume(bB) << endl;
        }
        outFile.close();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

std::vector<Monomer> HandlerPLY::createMonomers(const std::vector<std::unordered_set<int>>& clusters, const std::vector<Vertex>& globalVertices) const {
    std::vector<Monomer> monomers;
    for (const auto& cluster : clusters) {
        std::vector<Vertex> monomerVertices;
        for (int index : cluster) {
            monomerVertices.push_back(globalVertices[index]);
        }
        monomers.emplace_back(monomerVertices);
    }
    return monomers;
}