#include "Scene.h"
#include "Point.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>

void saveClustersDistribution(const std::unordered_map<int, int>& clusterSizes, std::ofstream& file, int removedCount) {
    if (!file) return;

    std::vector<std::pair<int, int>> sortedClusters(clusterSizes.begin(), clusterSizes.end());
    std::sort(sortedClusters.begin(), sortedClusters.end());

    file << "\nRemoved spheres: " << removedCount << "\n";
    file << "Cluster Size, Count\n";
    for (const auto& pair : sortedClusters) {
        file << pair.first << ", " << pair.second << "\n";
    }
}

void saveCoordinationNumbers(std::ofstream& file, int removedCount, double final) {
    if (!file) return;

    file << "\nRemoved spheres: " << removedCount << "\n";
    file << "After cluster processing, " << final << "\n";
}

void saveClusterCoordinationNumbers(std::ofstream& file, int removedCount, const std::unordered_map<int, double>& clusterCoordNumbers) {
    if (!file) return;

    std::vector<std::pair<int, double>> sortedClusters(clusterCoordNumbers.begin(), clusterCoordNumbers.end());
    std::sort(sortedClusters.begin(), sortedClusters.end());

    file << "\nRemoved spheres: " << removedCount << "\n";
    file << "Cluster Size, Average Coordination Number\n";
    for (const auto& pair : sortedClusters) {
        file << pair.first << ", " << pair.second << "\n"; // ÂÈÏÐÀÂËÅÍÎ: òåïåð çàïèñóþòüñÿ ÂÑ² êëàñòåðè
    }
}

int main() {
    double radius = 0.001;
    std::string filename = "spheres_positions.txt";
    int totalSpheres = 30000;
    int step = 1000;
    int minSpheres = 1000;

    std::ofstream clustersFile("clusters_distribution.txt");
    std::ofstream coordFile("coordination_numbers.txt");
    std::ofstream clusterCoordFile("cluster_coordination_numbers.txt");

    if (!clustersFile || !coordFile || !clusterCoordFile) {
        std::cerr << "Error opening output files.\n";
        return 1;
    }

    for (int removedCount = 0; removedCount <= totalSpheres; removedCount += step) {
        if (totalSpheres - removedCount <= minSpheres) step = 100;

        std::cout << "\nTesting removal of " << removedCount << " spheres...\n";

        Scene scene;
        std::vector<Point> points = readSpherePositions(filename);

        for (const auto& p : points) {
            scene.addSphere(p.x, p.y, p.z, radius);
        }

        scene.removeRandomSpheres(removedCount);

        auto clusters = scene.findClusters();
        double avgCoordFinal = scene.calculateAverageCoordinationNumber();
        auto clusterCoordNumbers = scene.calculateClusterCoordinationNumbers();

        std::unordered_map<int, int> clusterSizeCounts;
        for (const auto& cluster : clusters) {
            clusterSizeCounts[cluster.size()]++;
        }

        saveClustersDistribution(clusterSizeCounts, clustersFile, removedCount);
        saveCoordinationNumbers(coordFile, removedCount, avgCoordFinal);
        saveClusterCoordinationNumbers(clusterCoordFile, removedCount, clusterCoordNumbers);
    }

    std::cout << "\nAll results saved to files.\n";
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
