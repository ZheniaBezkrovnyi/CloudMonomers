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
        cout << 1 << endl;
        vector<Face> faces = ply.getFaces();
        cout << 2 << endl;

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
        ofstream outFile(getNameFile(_fileName) + "-" + to_string(static_cast<int>(procentBoundingBox * 100)) + "components.txt");
        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return;
        }
        for (int i = 0; i < vertices.size(); ++i) {
            if (!visited[i]) {
       
                unordered_set<int> vertIndexSet;
                set<int> faceIndexSet;

                separator.dfs(i, visited, vertices, faces, vertIndexSet, faceIndexSet);

                vector<Vertex> dividedVertices;
                a++;

                outFile << "" << endl;
                outFile << "Component " << a << endl;
                outFile << "Vects: " << endl;

                Vertex vLast = vertices[0];
                for (int idx : vertIndexSet) {
                    dividedVertices.push_back(vertices[idx]);
                    float distance = std::sqrt(
                        std::pow(vertices[idx].x - vLast.x, 2) +
                        std::pow(vertices[idx].y - vLast.y, 2) +
                        std::pow(vertices[idx].z - vLast.z, 2)
                    );
                    vLast = vertices[idx];
                    outFile << vertices[idx].x << "   " << vertices[idx].y << "   " << vertices[idx].z << endl;
                    outFile << distance << endl;
                }
                if (dividedVertices.size() < 3) break;
                outFile << "" << endl;
        
                vector<Face> dividedFaces;
                outFile << "Faces: " << endl;
                for (int idx : faceIndexSet) {
                    dividedFaces.push_back(faces[idx]);

                    outFile << faces[idx].v1 << "   " << faces[idx].v2 << "   " << faces[idx].v3 << endl;
                }

                monomers.emplace_back(dividedVertices, dividedFaces);
            }
        }
        outFile.close();

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