#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

class BinvoxReader {
    public:
    BinvoxReader(const std:: string& filename) : filename(filename){}

    bool readBinvox(){
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return false;
        }
    }




