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
        for (int i = 0; i < 100; ++i) {
            //cout << faces[i].v1 << "   " << faces[i].v2 << "   " << faces[i].v3 << endl;
        }
        for (int i = 0; i < 100; ++i) {
            cout << vertices[i].x << "   " << vertices[i].y << "   " << vertices[i].z << endl;
        }

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

        int a = 0;
        for (int i = 0; i < vertices.size(); ++i) {
            if (!visited[i]) {
                unordered_set<int> component;
                set<int> faceIndexSet;
                separator.dfs(i, visited, vertices, faces, component, faceIndexSet);

                vector<Vertex> dividedVertices;
                for (int idx : component) {
                    dividedVertices.push_back(vertices[idx]);
                }

                vector<Face> dividedFaces;
                for (int idx : faceIndexSet) {
                    dividedFaces.push_back(faces[idx]);

                    //cout << faces[idx].v1 << "   " << faces[idx].v2 << "   " << faces[idx].v3 << endl;
                }
                //cout << "-----------" << endl;

                monomers.emplace_back(dividedVertices, dividedFaces);
            }
        }

        double sumVolumeMonomers = 0.0;
        double sumVolumeCommon = 0.0;

        BoundingBox bB = findBoundingBox(vertices) * procentBoundingBox;
        sumVolumeCommon = findVolume(bB);

        for (size_t i = 0; i < monomers.size(); ++i) {
            //if (i < 100) {
                sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, bB);
            //}
        }
        cout << sumVolumeMonomers << " sumVolumeMonomers" << endl;
        cout << sumVolumeCommon << " sumVolumeCommon" << endl;
        cout << (sumVolumeMonomers / sumVolumeCommon * 100) << " %" << endl;
        cout << getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + ".txt" << endl;
        /*ofstream outFile(getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + ".txt");
        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }
        outFile << (sumVolumeMonomers / sumVolumeCommon * 100) << " %" << endl;
        for (size_t i = 0; i < monomers.size(); ++i) {
            outFile << "Monomer Index: " << i << "  volume: " << monomers[i].calculateClippedVolume(bB) << endl;
        }
        outFile.close();*/
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}