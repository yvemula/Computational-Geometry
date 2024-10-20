#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include <cmath>

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3, v4;  
};

void loadOBJ(const std::string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces) {
    std::ifstream objFile(filename);
    std::string line;

    if (!objFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    while (getline(objFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z});
        }
        else if (prefix == "f") {
            int v1, v2, v3, v4;
            iss >> v1 >> v2 >> v3 >> v4;
            faces.push_back({v1, v2, v3, v4});
        }
    }

    objFile.close();
}

std::tuple<float, float, float> calculateFaceCenter(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) {
    float centerX = (v1.x + v2.x + v3.x + v4.x) / 4.0f;
    float centerY = (v1.y + v2.y + v3.y + v4.y) / 4.0f;
    float centerZ = (v1.z + v2.z + v3.z + v4.z) / 4.0f;
    return std::make_tuple(centerX, centerY, centerZ);
}

std::tuple<float, float, float> calculateRelativePositionFromOrigin(float x, float y, float z) {
    return std::make_tuple(x, y, z);  
}

void processVoxel(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) {
    std::cout << "Voxel Vertices:" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "Vertex " << i + 1 << ": (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
    }

    std::cout << "\nVoxel Faces and their Relative Position from Origin (0, 0, 0):" << std::endl;
    for (size_t i = 0; i < faces.size(); ++i) {
        const Face& face = faces[i];
        auto faceCenter = calculateFaceCenter(vertices[face.v1 - 1], vertices[face.v2 - 1], vertices[face.v3 - 1], vertices[face.v4 - 1]);

        auto relativePosition = calculateRelativePositionFromOrigin(std::get<0>(faceCenter), std::get<1>(faceCenter), std::get<2>(faceCenter));

        std::cout << "Face " << i + 1 << " Center: (" 
                  << std::get<0>(faceCenter) << ", "
                  << std::get<1>(faceCenter) << ", "
                  << std::get<2>(faceCenter) << ")\n";

        std::cout << "Relative Position from Origin: (" 
                  << std::get<0>(relativePosition) << ", "
                  << std::get<1>(relativePosition) << ", "
                  << std::get<2>(relativePosition) << ")\n";
    }
}

int main() {
    std::string objFilename = "cube1by1.obj";  

    
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    loadOBJ(objFilename, vertices, faces);

    processVoxel(vertices, faces);

    return 0;
}
