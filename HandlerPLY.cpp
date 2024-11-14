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

void HandlerPLY::processManuallyBounds(const float& procentBoundingBox) const {
    try {
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();
        cout << faces.size() << endl;
        if (vertices.empty() || faces.empty()) {
            cerr << "No vertices or faces found in PLY file." << endl;
            return;
        }

        MonomerSeparator separator;
        int indexFace = 0;
        for (const auto& face : faces) {
            separator.addNeighbors(vertices, face, indexFace++);
        }

        vector<bool> visited(vertices.size(), false);
        vector<Monomer> monomers;
        cout << vertices.size() << "  a" << faces.size() << endl;
        int a = 0;
        ofstream outFile(getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + "components.txt");

        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }
        for (int i = 0; i < vertices.size(); i++) {
            outFile << vertices[i].neighbors.size() << endl;
            if (!visited[i]) {
                //outFile << i << endl;
                unordered_set<int> vertIndexSet;
                set<int> faceIndexSet;

                separator.dfs(i, visited, vertices, faces, vertIndexSet, faceIndexSet);

                vector<Vertex> dividedVertices;
                a++;

                Vertex vLast = vertices[0];
                for (int idx : vertIndexSet) {  dividedVertices.push_back(vertices[idx]); }

                if (dividedVertices.size() < 3) continue;

                vector<Face> dividedFaces;
                for (int idx : faceIndexSet) { dividedFaces.push_back(faces[idx]); }

                monomers.emplace_back(dividedVertices, dividedFaces);
            }
        }
        cout << a << "  a" << endl;
        outFile.close();

        double sumVolumeMonomers = 0.0;
        double sumVolumeCommon = 0.0;

        BoundingBox bB = findBoundingBox(vertices) * procentBoundingBox;
        sumVolumeCommon = findVolume(bB);
        cout << monomers.size() << endl;
        for (size_t i = 0; i < monomers.size(); ++i) {
            sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, bB);
        }
        cout << sumVolumeMonomers << " sumVolumeMonomers" << endl;
        cout << sumVolumeCommon << " sumVolumeCommon" << endl;
        cout << (sumVolumeMonomers / sumVolumeCommon * 100) << " %" << endl;
        cout << getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + ".txt" << endl;
        ofstream outFile2(getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + ".txt");
        if (!outFile2) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }
        outFile2 << (sumVolumeMonomers / sumVolumeCommon * 100) << " %" << endl;
        for (size_t i = 0; i < monomers.size(); ++i) {
            outFile2 << "Monomer Index: " << i << "  volume: " << monomers[i].calculateClippedVolume(vertices, bB) << endl;
        }
        outFile2.close();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}