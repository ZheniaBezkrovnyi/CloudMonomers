#include "MonomerSeparator.h"
#include <unordered_set>
#include <iostream>

using namespace std;

vector<vector<Face>> MonomerSeparator::separateMonomers(const vector<Face>& faces) {
    vector<vector<Face>> monomers;  
    vector<Face> currentMonomer;   

    for (size_t i = 0; i < faces.size(); ++i) {
        const Face& face = faces[i];

        if (face.v1 + 1 == face.v2 && face.v2 + 1 == face.v3) {
            if (!currentMonomer.empty() && currentMonomer.size() >= 4) {
                monomers.push_back(currentMonomer);
            }
            if (!currentMonomer.empty()) {
                currentMonomer.clear();
            }
        }

        currentMonomer.push_back(face);
    }

    if (!currentMonomer.empty() && currentMonomer.size() >= 4) {
        monomers.push_back(currentMonomer);
    }

    return monomers;
}