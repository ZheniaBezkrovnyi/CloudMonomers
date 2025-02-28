#include <string>
#include "Sphere.h"
#include "Point.h"
#include <iostream>
#include "Scene.h"

using namespace std;

int main() {
    double radius = 0.001;
    std::string filename = "spheres_positions.txt";
    Scene scene;
    std::vector<Point> points = readSpherePositions(filename);

    for (const auto& p : points) {
        scene.addSphere(p.x, p.y, p.z, radius);
    }

    std::cout << "Initial scene:\n";
    scene.printScene();

    auto clusters1 = scene.findClusters();
    std::cout << "Clusters before removal: " << clusters1.size() << "\n";

    scene.removeRandomSpheres(25000);

    auto clusters2 = scene.findClusters();
    std::cout << "Clusters after random removal: " << clusters2.size() << "\n";

    int X = 3;

    scene.processClusters(X);

    std::cout << "\nAfter processing clusters:\n";
    scene.printScene();

    auto clusters3 = scene.findClusters();
    std::cout << "Remaining clusters: " << clusters3.size() << "\n";

    return 0;

}



    /*vector<std::string> fileNames = {
        "C:/All/Diploma/PLY/Example/Cubes Size 12-14 Margin 1-7 density026_ascii.ply",
        "C:/All/Diploma/PLY/Example/Cubes Size 12-14 Margin 05-4 density038.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 1-6 density023ply.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 2 density025.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 02 density045.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 05 density037.ply",
        "C:/All/Diploma/PLY/Example/layer cubes.ply",
        "C:/All/Diploma/PLY/Example/layer octahedra.ply",
        "C:/All/Diploma/PLY/Example/layer_octahedra_dense.ply",
        "C:/All/Diploma/PLY/Example/layer_tetrahedra.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 1 density031.ply",
        "C:/All/Diploma/PLY/Example/icosphere_1.ply"
    };
    "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 1 density031.ply",
    "C:/All/Diploma/PLY/Example/icosphere_1.ply",

    for (const auto& fileName : fileNames) {
        cout << "Processing file: " << fileName << std::endl;

        HandlerPLY handler(fileName);
        handler.processManuallyBounds();
    }

    return 0;*/
