#include "PLY.h"
#include "plycpp.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace plycpp;

PLY::PLY(const string& file) : fileName(file) {}


vector<Vertex> PLY::getVertices() const {
    PLYData plyData;
    load(fileName, plyData);  

    vector<Vertex> vertices;

    if (plyData.has_key("vertex")) {
        auto plyVertex = plyData["vertex"];
        vector<shared_ptr<const PropertyArray>> vertexProperties = {
            plyVertex->properties["x"],
            plyVertex->properties["y"],
            plyVertex->properties["z"]
        };

        for (size_t i = 0; i < plyVertex->size(); ++i) {
            Vertex vertex;
            vertex.x = vertexProperties[0]->at<float>(i);
            vertex.y = vertexProperties[1]->at<float>(i);
            vertex.z = vertexProperties[2]->at<float>(i);
            vertices.push_back(vertex);
        }
    }

    return vertices;
}


vector<Face> PLY::getFaces() const {
    PLYData plyData;
    load(fileName, plyData);

    vector<Face> faces;

    if (plyData.has_key("face")) {
        auto plyFace = plyData["face"];
        auto faceProperty = plyFace->properties["vertex_indices"];


        for (size_t i = 0; i < plyFace->size(); ++i) {
            Face face;

            face.v1 = faceProperty->at<uint32_t>(i * 3 + 0);
            face.v2 = faceProperty->at<uint32_t>(i * 3 + 1);
            face.v3 = faceProperty->at<uint32_t>(i * 3 + 2);

            faces.push_back(face);
        }
    }

    return faces;
}
