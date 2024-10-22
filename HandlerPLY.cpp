#include "HandlerPLY.h"
#include "PLY.h"  // ��� ������ � PLY �������
#include "Monomer.h"  // ��� ���������� ��'��� ��������
#include "MonomerSeparator.h"  // ��� ��������� ������ �� ��������
#include <iostream>
#include <unordered_set>  // ��� ��������� ��������� ������

using namespace std;

// �����������, �� �������� ��'��� HandlerPLY � ������ �����
HandlerPLY::HandlerPLY(const string& fileName) : _fileName(fileName) {}

// �������� ����� ��� ������� PLY �����
void HandlerPLY::process() const {
    try {
        // ������������ PLY �����
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();

        if (vertices.empty() || faces.empty()) {
            cout << "No vertices or faces found in PLY file." << endl;
            return;
        }

        // ������������ MonomerSeparator ��� ��������� �� ��������
        MonomerSeparator separator;
        vector<vector<Face>> monomersFaces = separator.separateMonomers(faces, vertices);

        // ������� ������� ��������
        for (size_t i = 0; i < monomersFaces.size(); ++i) {
            // �������� ������� ������� ��� ������� ��������
            vector<Vertex> monomerVertices = Monomer::extractVerticesFromFaces(monomersFaces[i], vertices);

            // ���� ������� �� ����� �� 4 �������, ���������� ����
            if (monomerVertices.size() < 4) {
                cout << "Monomer " << i + 1 << " has less than 4 vertices, skipping..." << endl;
                continue;
            }

            Monomer monomer(monomerVertices);

            // ��������� ���������� ��� ������� ��������
            cout << "Monomer " << i + 1 << " vertices:" << endl;
            for (const Vertex& vertex : monomerVertices) {
                cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl;
            }

            // ��������� ���������� ��� ���� ��������
            cout << "Monomer " << i + 1 << " faces:" << endl;
            for (const Face& face : monomersFaces[i]) {
                cout << "Face: (" << face.v1 << ", " << face.v2 << ", " << face.v3 << ")" << endl;
            }

            // ���������� ��'��� ��������� ��������
            double volume = monomer.calculateVolume();
            cout << "Monomer " << i + 1 << " has volume: " << volume << endl;

            if (i != 0) continue;
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}
