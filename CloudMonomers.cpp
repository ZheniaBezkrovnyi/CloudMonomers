#include "HandlerPLY.h"
#include <string>

using namespace std;

int main() {
    //string fileName = "C:/All/Diploma/PLY/Example/layer_octahedra_dense.ply";
    //string fileName = "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 2 density025.ply";
    //string fileName = "C:/All/Diploma/PLY/Example/layer_tetrahedra.ply";
    //string fileName = "C:/All/Diploma/PLY/Example/Cubes Size 12-14 Margin 05-4 density038.ply";
    string fileName = "C:/All/Diploma/PLY/Example/GRF Size 12-14 Margin 1-6 density023ply.ply";



    HandlerPLY handler(fileName);  // Створення об'єкта HandlerPLY з назвою файлу
    handler.process();  // Виклик методу process для обробки PLY файлу

    return 0;
}
