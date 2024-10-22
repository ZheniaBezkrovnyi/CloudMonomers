#include "HandlerPLY.h"
#include "PLY.h"  // Для роботи з PLY файлами
#include "Monomer.h"  // Для обчислення об'єму мономерів
#include "MonomerSeparator.h"  // Для розділення вершин на мономери
#include <iostream>
#include <unordered_set>  // Для зберігання унікальних вершин

using namespace std;

// Конструктор, що ініціалізує об'єкт HandlerPLY з назвою файлу
HandlerPLY::HandlerPLY(const string& fileName) : _fileName(fileName) {}

// Основний метод для обробки PLY файлу
void HandlerPLY::process() const {
    try {
        // Завантаження PLY файлу
        PLY ply(_fileName);
        vector<Vertex> vertices = ply.getVertices();
        vector<Face> faces = ply.getFaces();

        if (vertices.empty() || faces.empty()) {
            cout << "No vertices or faces found in PLY file." << endl;
            return;
        }

        // Використання MonomerSeparator для розділення на мономери
        MonomerSeparator separator;
        vector<vector<Face>> monomersFaces = separator.separateMonomers(faces, vertices);

        // Обробка кожного мономера
        for (size_t i = 0; i < monomersFaces.size(); ++i) {
            // Витягуємо унікальні вершини для кожного мономера
            vector<Vertex> monomerVertices = Monomer::extractVerticesFromFaces(monomersFaces[i], vertices);

            // Якщо мономер має менше ніж 4 вершини, пропускаємо його
            if (monomerVertices.size() < 4) {
                cout << "Monomer " << i + 1 << " has less than 4 vertices, skipping..." << endl;
                continue;
            }

            Monomer monomer(monomerVertices);

            // Виведення інформації про вершини мономера
            cout << "Monomer " << i + 1 << " vertices:" << endl;
            for (const Vertex& vertex : monomerVertices) {
                cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl;
            }

            // Виведення інформації про грані мономера
            cout << "Monomer " << i + 1 << " faces:" << endl;
            for (const Face& face : monomersFaces[i]) {
                cout << "Face: (" << face.v1 << ", " << face.v2 << ", " << face.v3 << ")" << endl;
            }

            // Обчислення об'єму поточного мономера
            double volume = monomer.calculateVolume();
            cout << "Monomer " << i + 1 << " has volume: " << volume << endl;

            if (i != 0) continue;
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}
