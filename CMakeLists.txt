cmake_minimum_required(VERSION 3.1)
project(SurfaceMeshSimplification)

# Find CGAL and Boost
find_package(CGAL REQUIRED)
find_package(Boost REQUIRED)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Add CGAL include directories
include_directories(${CGAL_INCLUDE_DIRS})

# Add the executable and link to CGAL
add_executable(SurfaceMeshSimplification main1.cpp)
target_link_libraries(SurfaceMeshSimplification CGAL::CGAL)

