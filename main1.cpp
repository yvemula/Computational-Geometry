#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <iostream>
#include <fstream>
#include <vector>

// CGAL types
typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> Surface_mesh;
typedef Kernel::Point_3 Point3D;

// Custom 2D point structure
struct Point2D {
    double x, y;
};

// Function to project 3D points to 2D
std::vector<Point2D> projectTo2D(const Surface_mesh& mesh) {
    std::vector<Point2D> projected_points;
    for (auto vertex : mesh.vertices()) {
        const auto& point = mesh.point(vertex);
        // Example: Project onto XY plane (ignore Z)
        projected_points.push_back({ point.x(), point.y() });
    }
    return projected_points;
}

// Function to simplify the mesh using CGAL
Surface_mesh simplifyMesh(Surface_mesh& mesh, std::size_t target_faces) {
    CGAL::Surface_mesh_simplification::Count_stop_predicate<Surface_mesh> stop(target_faces);
    int num_collapsed = CGAL::Surface_mesh_simplification::edge_collapse(mesh, stop);
    std::cout << "Simplified mesh, collapsed " << num_collapsed << " edges." << std::endl;
    return mesh;
}

// Main function
int main() {
    // Step 1: Load the mesh from an OBJ file
    std::string objFile =  "/mnt/c/Users/Yathin Vemula/Dropbox/PC/Desktop/cgal_mesh_simplification/Koltuk.obj"; // Replace with your file
    Surface_mesh mesh;

    std::ifstream input(objFile);
    if (!input || !(input >> mesh)) {
        std::cerr << "Error: Cannot open or load the mesh from file." << std::endl;
        return EXIT_FAILURE;
    }

    // Step 2: Simplify Mesh
    std::size_t target_faces = 100; // Set the desired number of faces
    Surface_mesh simplified_mesh = simplifyMesh(mesh, target_faces);

    // Step 3: Project 3D Surface to 2D
    std::vector<Point2D> planar_surface = projectTo2D(simplified_mesh);
    std::cout << "Projected mesh to 2D." << std::endl;

    // Step 4: Save the simplified mesh to an OBJ file
    std::ofstream output("simplified_mesh.obj");
    output << simplified_mesh;
    output.close();

    // Step 5: Output 2D projection
    std::ofstream output2D("projected_surface.txt");
    for (const auto& point : planar_surface) {
        output2D << point.x << " " << point.y << std::endl;
    }
    output2D.close();

    std::cout << "Processing complete. Mesh saved to 'simplified_mesh.obj' and 2D projection saved to 'projected_surface.txt'." << std::endl;
    return 0;
}
