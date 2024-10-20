#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

// Load the 2D voxel projection from a text file
std::vector<std::string> loadVoxelProjection(const std::string& filePath) {
    std::vector<std::string> grid;
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Could not open the file!" << std::endl;
        return grid;
    }

    std::string line;
    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    return grid;
}

int main() {
    // Load voxel projection from file
    std::string filePath = "planar_cube.txt";
    std::vector<std::string> voxelGrid = loadVoxelProjection(filePath);

    if (voxelGrid.empty()) {
        std::cerr << "Failed to load voxel projection." << std::endl;
        return 1;
    }

    const int cellSize = 20;  // Size of each cell in the grid
    const int rows = voxelGrid.size();
    const int cols = voxelGrid[0].size();

    // Create a window
    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "Voxel Projection");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Render the voxel grid
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(x * cellSize, y * cellSize);

                if (voxelGrid[y][x] == '#') {
                    cell.setFillColor(sf::Color::Black);  // Filled voxel
                } else {
                    cell.setFillColor(sf::Color::White);  // Empty voxel
                }

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}
