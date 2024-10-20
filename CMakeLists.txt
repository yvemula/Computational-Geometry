cmake_minimum_required(VERSION 3.5)
project(VoxelToMesh)

# Require C++14
set(CMAKE_CXX_STANDARD 14)


# Add the executable
add_executable(voxel_to_mesh voxelmesh.cpp)