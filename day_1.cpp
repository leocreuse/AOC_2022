#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    auto max_elf_val = 0;
    std::vector<int> elf_vals;
    elf_vals.push_back(0);
    while (std::getline (input_file, line)){
        if(line.empty()){
            if(elf_vals.back() > max_elf_val)
                max_elf_val = elf_vals.back();
            elf_vals.push_back(0);
        } else {
            elf_vals.back() += std::stoi(line);
        }
    }
    std::cout << "Part 1: " << max_elf_val << std::endl;

    std::sort(elf_vals.begin(), elf_vals.end());
    std::reverse(elf_vals.begin(), elf_vals.end());

    std::cout << "Part 2: " << elf_vals[0] + elf_vals [1] + elf_vals [2] << std::endl;

}
