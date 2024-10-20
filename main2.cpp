#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <set>

// Vertex structure to hold coordinates
struct Vertex {
    float x, y, z;
};

// Face structure to hold vertex indices
struct Face {
    std::vector<int> vertices;
};

// Voxel grid size and resolution
const int VOXEL_GRID_SIZE = 32; // 32x32x32 voxel grid
bool voxel_grid[VOXEL_GRID_SIZE][VOXEL_GRID_SIZE][VOXEL_GRID_SIZE];

// Function to read OBJ file
std::vector<Vertex> read_obj(const std::string& file_path, std::vector<Face>& faces) {
    std::ifstream obj_file(file_path);
    std::string line;
    std::vector<Vertex> vertices;

    if (!obj_file) {
        std::cerr << "Could not open the file!" << std::endl;
        return vertices;
    }

    while (std::getline(obj_file, line)) {
        std::istringstream line_stream(line);
        std::string prefix;
        line_stream >> prefix;

        if (prefix == "v") {
            // Parse vertex
            Vertex v;
            line_stream >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (prefix == "f") {
            // Parse face
            Face face;
            std::string vertex_info;
            while (line_stream >> vertex_info) {
                std::stringstream vertex_stream(vertex_info);
                std::string vertex_index;
                std::getline(vertex_stream, vertex_index, '/');
                face.vertices.push_back(std::stoi(vertex_index));
            }
            faces.push_back(face);
        }
    }
    obj_file.close();
    return vertices;
}

// Normalize vertices into a voxel grid space
void normalize_vertices(std::vector<Vertex>& vertices) {
    float min_x = 1e9, min_y = 1e9, min_z = 1e9;
    float max_x = -1e9, max_y = -1e9, max_z = -1e9;

    // Find min/max of x, y, z to normalize
    for (const auto& v : vertices) {
        if (v.x < min_x) min_x = v.x;
        if (v.y < min_y) min_y = v.y;
        if (v.z < min_z) min_z = v.z;
        if (v.x > max_x) max_x = v.x;
        if (v.y > max_y) max_y = v.y;
        if (v.z > max_z) max_z = v.z;
    }

    float scale_x = VOXEL_GRID_SIZE / (max_x - min_x);
    float scale_y = VOXEL_GRID_SIZE / (max_y - min_y);
    float scale_z = VOXEL_GRID_SIZE / (max_z - min_z);

    // Normalize vertices into voxel grid size (0 to VOXEL_GRID_SIZE-1)
    for (auto& v : vertices) {
        v.x = (v.x - min_x) * scale_x;
        v.y = (v.y - min_y) * scale_y;
        v.z = (v.z - min_z) * scale_z;
    }
}

// Function to fill voxels for each vertex
void fill_voxels(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) {
    // Initialize voxel grid to empty
    std::fill(&voxel_grid[0][0][0], &voxel_grid[0][0][0] + sizeof(voxel_grid), false);

    // Simple voxel filling: fill based on vertex positions
    for (const auto& v : vertices) {
        int x = std::round(v.x);
        int y = std::round(v.y);
        int z = std::round(v.z);
        if (x >= 0 && x < VOXEL_GRID_SIZE && y >= 0 && y < VOXEL_GRID_SIZE && z >= 0 && z < VOXEL_GRID_SIZE) {
            voxel_grid[x][y][z] = true;
        }
    }
}

// Project voxel grid to 2D (XY plane projection)
void project_to_xy() {
    std::ofstream output_file("planar_cube.txt");
    for (int x = 0; x < VOXEL_GRID_SIZE; ++x) {
        for (int y = 0; y < VOXEL_GRID_SIZE; ++y) {
            bool filled = false;
            for (int z = 0; z < VOXEL_GRID_SIZE; ++z) {
                if (voxel_grid[x][y][z]) {
                    filled = true;
                    break;
                }
            }
            if (filled) {
                output_file << "#";
            } else {
                output_file << ".";
            }
        }
        output_file << "\n";
    }
    output_file.close();
    std::cout << "Planar projection saved to planar_cube.txt" << std::endl;
}

int main() {
    std::string input_file = "Koltuk.obj";  // Input OBJ file
    std::vector<Face> faces;
    std::vector<Vertex> vertices = read_obj(input_file, faces);

    if (vertices.empty()) {
        std::cerr << "No vertices were read from the file!" << std::endl;
        return 1;
    }

    // Normalize vertices into voxel grid space
    normalize_vertices(vertices);

    // Fill voxels based on normalized vertices
    fill_voxels(vertices, faces);

    // Project voxelized object to a 2D plane
    project_to_xy();

    return 0;
}

