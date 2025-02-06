#include "HandlerPLY.h"
#include "PLY.h"
#include <iostream>
#include <unordered_set> 
#include "MonomerSeparator.h"
#include <fstream>
#include "BoundingBox.h"
#include "HandlerString.h"
#include <iomanip>

using namespace std;

HandlerPLY::HandlerPLY(const string& fileName) : _fileName(fileName) {}

int HandlerPLY::processManuallyBounds() const {
    try {
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();
        cout << faces.size() << "  faces" << endl;
        cout << vertices.size() << "  verts" << endl;
        if (vertices.empty() || faces.empty()) {
            cerr << "No vertices or faces found in PLY file." << endl;
            return 0;
        }

        MonomerSeparator separator;
        int indexFace = 0;
        for (const auto& face : faces) {
            separator.addNeighbors(vertices, face, indexFace++);
        }

        vector<bool> visited(vertices.size(), false);
        vector<Monomer> monomers;


        for (int i = 0; i < vertices.size(); i++) {
            if (!visited[i]) {
                unordered_set<int> vertIndexSet;
                set<int> faceIndexSet;

                separator.dfs(i, visited, vertices, faces, vertIndexSet, faceIndexSet);

                vector<Vertex> dividedVertices;

                Vertex vLast = vertices[0];
                for (int idx : vertIndexSet) {  dividedVertices.push_back(vertices[idx]); }

                if (dividedVertices.size() < 3) continue;

                vector<Face> dividedFaces;
                for (int idx : faceIndexSet) { dividedFaces.push_back(faces[idx]); }

                monomers.emplace_back(dividedVertices, dividedFaces);
            }
        }

        ofstream outFile(getNameFile(_fileName) + "_divided.txt");

        if (!outFile) {
            cerr << "Cannot open file for writing." << endl;
            return 0;
        }
        outFile << faces.size() << "  faces" << endl;
        outFile << vertices.size() << "  verts" << endl;
        //
        double _sumVolumeMonomers = 0.0;
        double _sumVolumeCommon = 0.0;
        outFile << monomers.size() << "  monomers" << endl;
        outFile << endl;
        outFile << endl;
        BoundingBox _bB = findBoundingBox(vertices).transformByAxis(0, 1, 0, 1, 0, 1);

        _sumVolumeCommon = findVolume(_bB);


        for (size_t i = 0; i < monomers.size(); ++i) {
            _sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, _bB);
        }
        //cout << monomers[1].innerVertices.size() << endl;

        cout << (_sumVolumeMonomers / _sumVolumeCommon * 100) << " % Main" << endl;
        outFile << (_sumVolumeMonomers / _sumVolumeCommon * 100) << " %" << endl;
        outFile << endl;
        

        int side =  5;

        outFile << "X" << endl;
        for (int x = 0; x < side; x += 1) {
            float minX = (float)x / side, maxX = (float)(x + 1) / side;

            double sumVolumeMonomers = 0.0;
            double sumVolumeCommon = 0.0;

            BoundingBox bB = findBoundingBox(vertices).transformByAxis(minX, maxX, 0, 1, 0, 1);
            sumVolumeCommon = findVolume(bB);

            for (size_t i = 0; i < monomers.size(); ++i) {
                sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, bB);
            }
            float porous = sumVolumeMonomers / sumVolumeCommon * 100;
            cout << porous << " %" << endl;

            outFile << porous << " %" << endl;
            outFile << endl;
        }
        ///////

        outFile << "Y" << endl;
        for (int y = 0; y < side; y += 1) {
            float minY = (float)y / side, maxY = (float)(y + 1) / side;

            double sumVolumeMonomers = 0.0;
            double sumVolumeCommon = 0.0;

            BoundingBox bB = findBoundingBox(vertices).transformByAxis(0, 1, minY, maxY, 0, 1);
            sumVolumeCommon = findVolume(bB);

            for (size_t i = 0; i < monomers.size(); ++i) {
                sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, bB);
            }
            float porous = sumVolumeMonomers / sumVolumeCommon * 100;
            cout << porous << " %" << endl;

            outFile << porous << " %" << endl;
            outFile << endl;
        }
        //////////////

        outFile << "Z" << endl;
        for (int z = 0; z < side; z += 1) {
            float minZ = (float)z / side, maxZ = (float)(z + 1) / side;

            double sumVolumeMonomers = 0.0;
            double sumVolumeCommon = 0.0;

            BoundingBox bB = findBoundingBox(vertices).transformByAxis(0, 1, 0, 1, minZ, maxZ);
            sumVolumeCommon = findVolume(bB);

            for (size_t i = 0; i < monomers.size(); ++i) {
                sumVolumeMonomers += monomers[i].calculateClippedVolume(vertices, bB);
            }
            float porous = sumVolumeMonomers / sumVolumeCommon * 100;
            cout << porous << " %" << endl;

            outFile << porous << " %" << endl;
            outFile << endl;
        }
        outFile.close();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}