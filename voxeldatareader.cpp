#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_set>  

using namespace std;

#pragma pack(push, 1)
struct Chunk {
    char id[4];             
    int32_t contentSize;     
    int32_t childrenSize;    
};
#pragma pack(pop)

struct Voxel {
    uint8_t x, y, z, colorIndex;
};

struct Size {
    int32_t x, y, z;
};

unordered_set<uint8_t> usedColorIndices;

void readRGBA(ifstream &file) {
    cout << "\nColor:\n";
    vector<uint32_t> palette(256);
    file.read(reinterpret_cast<char*>(palette.data()), 256 * sizeof(uint32_t));

    for (uint8_t colorIndex : usedColorIndices) {
        uint32_t color = palette[colorIndex];
        cout << "Color Index " << static_cast<int>(colorIndex) << ": "
             << "R: " << ((color >> 24) & 0xFF) << " "
             << "G: " << ((color >> 16) & 0xFF) << " "
             << "B: " << ((color >> 8) & 0xFF) << " "
             << "A: " << (color & 0xFF) << "\n";
    }
}

void readXYZI(ifstream &file) {
    int32_t numVoxels;
    file.read(reinterpret_cast<char*>(&numVoxels), sizeof(numVoxels));

    vector<Voxel> voxels(numVoxels);
    file.read(reinterpret_cast<char*>(voxels.data()), numVoxels * sizeof(Voxel));

    for (int i = 0; i < numVoxels; i++) {
        const auto &voxel = voxels[i];
        cout << "Voxel " << i << ": X=" << static_cast<int>(voxel.x) 
             << " Y=" << static_cast<int>(voxel.y)
             << " Z=" << static_cast<int>(voxel.z) 
             << " Color Index=" << static_cast<int>(voxel.colorIndex) << "\n";

        usedColorIndices.insert(voxel.colorIndex);
    }
}

void readSIZE(ifstream &file) {
    Size size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    cout << "Model Dimensions: X=" << size.x << " Y=" << size.z << " Z=" << size.y << "\nVoxels:\n";
}

void processChunk(ifstream &file, const Chunk &chunk) {
    string id(chunk.id, 4);
    if (id == "SIZE") {
        readSIZE(file);
    } else if (id == "XYZI") {
        readXYZI(file);
    } else if (id == "RGBA") {
        readRGBA(file);
    } else {
        file.seekg(chunk.contentSize, ios::cur); 
    }
}

void readChunks(ifstream &file, int remainingBytes) {
    while (remainingBytes > 0) {
        Chunk chunk;
        file.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));

        int chunkDataSize = chunk.contentSize + chunk.childrenSize;
        processChunk(file, chunk);

        remainingBytes -= (sizeof(chunk) + chunkDataSize);
        if (chunk.childrenSize > 0) {
            readChunks(file, chunk.childrenSize);
        }
    }
}

void readVoxFile(const string &filePath) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    char magic[4];
    int32_t version;
    file.read(magic, 4);
    file.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (string(magic, 4) != "VOX ") {
        cerr << "Invalid file format, not a .vox file.\n";
        return;
    }

    Chunk mainChunk;
    file.read(reinterpret_cast<char*>(&mainChunk), sizeof(mainChunk));

    if (string(mainChunk.id, 4) != "MAIN") {
        cerr << "Invalid format, MAIN chunk missing.\n";
        return;
    }
    readChunks(file, mainChunk.childrenSize);

    file.close();
}

int main() {
    string filePath = "four.vox"; 
    readVoxFile(filePath);
    return 0;
}
