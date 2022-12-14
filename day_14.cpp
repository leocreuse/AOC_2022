#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum Cell_State {Empty, Rock, Sand};

struct rock_rec {
    int x;
    int y;
};

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    
    std::vector<rock_rec> rocks;

    while (std::getline (input_file, line)){
    }
}
