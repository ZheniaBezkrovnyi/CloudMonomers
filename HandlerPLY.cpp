#include "HandlerPLY.h"
#include "PLY.h"  
#include "Monomer.h"  
#include "MonomerSeparator.h" 
#include <iostream>
#include <unordered_set> 

using namespace std;

HandlerPLY::HandlerPLY(const string& fileName) : _fileName(fileName) {}

void HandlerPLY::process() const {
    try {
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();

        if (vertices.empty() || faces.empty()) {
            cerr << "No vertices or faces found in PLY file." << endl;
            return;
        }

        MonomerSeparator separator;
        vector<vector<Face>> monomersFaces = separator.separateMonomers(faces);

        for (size_t i = 0; i < monomersFaces.size(); ++i) {
            Monomer monomer(monomersFaces[i], vertices);

            //cout << "Monomer " << i + 1 << " vertices:" << endl;
            vector<Vertex> monomerVertices = monomer.extractVerticesFromFaces(monomersFaces[i], vertices);
            for (const Vertex& vertex : monomerVertices) {
                //cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl;
            }

            //cout << "Monomer " << i + 1 << " faces:" << endl;
            for (const Face& face : monomersFaces[i]) {
                //cout << "Face: (" << face.v1 << ", " << face.v2 << ", " << face.v3 << ")" << endl;
            }

            double volume = monomer.calculateVolume();

            if (volume < 0.0001 || volume > 4000) {
                for (const Vertex& vertex : monomerVertices) {
                    cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl;
                }

                //cout << "Monomer " << i + 1 << " faces:" << endl;
                for (const Face& face : monomersFaces[i]) {
                    cout << "Face: (" << face.v1 << ", " << face.v2 << ", " << face.v3 << ")" << endl;
                }

                cout << "Monomer " << i + 1 << " has volume: " << volume << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}
