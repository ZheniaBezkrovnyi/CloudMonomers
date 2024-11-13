#include "HandlerPLY.h"
#include <string>

using namespace std;

int main() {
    vector<std::string> fileNames = {
        "C:/All/Diploma/PLY/Example/Cubes Size 12-14 Margin 1-7 density026_ascii.ply",
        /*"C:/All/Diploma/PLY/Example/Cubes Size 12-14 Margin 05-4 density038.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 1-6 density023ply.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 2 density025.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 02 density045.ply",
        "C:/All/Diploma/PLY/Example/GRF Size 14 Margin 05 density037.ply",
        "C:/All/Diploma/PLY/Example/layer cubes.ply",
        "C:/All/Diploma/PLY/Example/layer octahedra.ply",
        "C:/All/Diploma/PLY/Example/layer_octahedra_dense.ply",
        "C:/All/Diploma/PLY/Example/layer_tetrahedra.ply",*/
    };
    //"C:/All/Diploma/PLY/Example/GRF Size 14 Margin 1 density031.ply",
    //"C:/All/Diploma/PLY/Example/icosphere_1.ply",

    for (const auto& fileName : fileNames) {

        for (float i = 1; i <= 1; i += 0.01) {
            HandlerPLY handler(fileName);
            handler.processManuallyBounds(i);
        }

        cout << "Processing file: " << fileName << std::endl;
    }

    return 0;
}
