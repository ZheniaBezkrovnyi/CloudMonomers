#include "PLY.h"
#include "plycpp.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace plycpp;

PLY::PLY(const string& file) : fileName(file), typeIndex(typeid(void)) {}


vector<Vertex> PLY::getVertices() const {
    PLYData plyData;
    load(fileName, plyData);

    vector<Vertex> vertices;
    for (const auto& element : plyData) {
        if (element.key == "vertex") {
            auto plyVertex = element.data;
            int size = plyVertex->size();

            for (size_t i = 0; i < size; i += 3) {
                if (i + 2 >= size) break;

                Vertex vertex;

                vertex.x = plyVertex->properties["x"]->at<float>(i);
                vertex.y = plyVertex->properties["y"]->at<float>(i + 1);
                vertex.z = plyVertex->properties["z"]->at<float>(i + 2);
                vertices.push_back(vertex);
            }
        }
    }
    return vertices;
}


vector<Face> PLY::getFaces() const {
    PLYData plyData;
    load(fileName, plyData);

    vector<Face> faces;
    for (const auto& element : plyData) {
        for (const auto& prop : element.data->properties)
        {
            if (prop.key == string("vertex_indices")) {  
                typeIndex = prop.data->type;  
            }
        }
    }
    for (const auto& element : plyData) {
        if (element.key == "face") {
            auto plyFace = plyData["face"];
            auto faceProperty = plyFace->properties["vertex_indices"];
            int I = 0;

            bool isUnsigned = typeIndex.name() == string("unsigned int");

            for (size_t i = 0; i < plyFace->size(); i+=3) {
                Face face;

                if (isUnsigned) {
                    face.v1 = static_cast<int>(faceProperty->at<unsigned int>(i + 0));
                    face.v2 = static_cast<int>(faceProperty->at<unsigned int>(i + 1));
                    face.v3 = static_cast<int>(faceProperty->at<unsigned int>(i + 2));
                }
                else {
                    face.v1 = faceProperty->at<int>(i + 0);
                    face.v2 = faceProperty->at<int>(i + 1);
                    face.v3 = faceProperty->at<int>(i + 2);
                }
                faces.push_back(face);
            }
        }
    }

    return faces;
}
