#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Mesh;

struct Voxel {
    Point_3 min_corner;
    double size;
    
    std::vector<Point_3> get_vertices() const {
        std::vector<Point_3> vertices;
        double x = min_corner.x();
        double y = min_corner.y();
        double z = min_corner.z();
        double s = size;
        vertices.push_back(Point_3(x, y, z));          // Vertex 0
        vertices.push_back(Point_3(x + s, y, z));      // Vertex 1
        vertices.push_back(Point_3(x + s, y + s, z));  // Vertex 2
        vertices.push_back(Point_3(x, y + s, z));      // Vertex 3
        vertices.push_back(Point_3(x, y, z + s));      // Vertex 4
        vertices.push_back(Point_3(x + s, y, z + s));  // Vertex 5
        vertices.push_back(Point_3(x + s, y + s, z + s));  // Vertex 6
        vertices.push_back(Point_3(x, y + s, z + s));      // Vertex 7
        return vertices;
    }
};

void write_voxel_obj(const std::string &filename, const std::vector<Voxel> &voxels) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    int vertex_index = 1; 
    for (const auto &voxel : voxels) {
        std::vector<Point_3> vertices = voxel.get_vertices();
        
        
        for (const auto &v : vertices) {
            out << "v " << v.x() << " " << v.y() << " " << v.z() << std::endl;
        }
        
        out << "f " << vertex_index << " " << vertex_index + 1 << " " << vertex_index + 2 << " " << vertex_index + 3 << std::endl;
        out << "f " << vertex_index + 4 << " " << vertex_index + 5 << " " << vertex_index + 6 << " " << vertex_index + 7 << std::endl;
        out << "f " << vertex_index << " " << vertex_index + 1 << " " << vertex_index + 5 << " " << vertex_index + 4 << std::endl;
        out << "f " << vertex_index + 1 << " " << vertex_index + 2 << " " << vertex_index + 6 << " " << vertex_index + 5 << std::endl;
        out << "f " << vertex_index + 2 << " " << vertex_index + 3 << " " << vertex_index + 7 << " " << vertex_index + 6 << std::endl;
        out << "f " << vertex_index + 3 << " " << vertex_index << " " << vertex_index + 4 << " " << vertex_index + 7 << std::endl;

        vertex_index += 8;
    }

    out.close();
    std::cout << "Voxelized OBJ file written to " << filename << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: voxelizer <input.obj>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = "output_voxelized.obj";
    
    Mesh mesh;
    if (!CGAL::Polygon_mesh_processing::IO::read_polygon_mesh(input_file, mesh)) {
        std::cerr << "Failed to read OBJ file." << std::endl;
        return 1;
    }

    // Define voxel grid parameters
    double voxel_size = 1.0;
    std::vector<Voxel> voxels;

    for (double x = 0; x < 10; x += voxel_size) {
        for (double y = 0; y < 10; y += voxel_size) {
            for (double z = 0; z < 10; z += voxel_size) {
             
                voxels.push_back(Voxel{Point_3(x, y, z), voxel_size});
            }
        }
    }

    // Write voxelized mesh to output OBJ file
    write_voxel_obj(output_file, voxels);

    return 0;
}
