#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

int score_1 (int elf_val, int my_val);

int score_2 (int elf_val, int my_val);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    auto part_1_score = 0;
    auto part_2_score = 0;
    while(std::getline(input_file, line)){
        part_1_score += score_1 (line[0], line[2]);
        part_2_score += score_2 (line[0], line[2]);
    }
    std::cout << "Part 1: " << part_1_score << std::endl;
    std::cout << "Part 2: " << part_2_score << std::endl;
}

int score_1 (int elf_val, int my_val){
    auto actual_val = 'A' + my_val - 'X';
    auto pick_score = actual_val - 'A' + 1;
    auto battle_score = 0;
    if(elf_val == actual_val){
        battle_score = 3;
    } else if(elf_val == 'A'){
        battle_score = actual_val == 'B' ? 6 : 0;
    } else if (elf_val == 'B') {
        battle_score = actual_val == 'C' ? 6 : 0;
    } else {
        battle_score = actual_val == 'A' ? 6 : 0;
    }
    return battle_score + pick_score;
}

int score_2 (int elf_val, int my_val) {
    auto battle_score = (my_val - 'X') * 3;
    auto my_play = 'A';
    if(my_val == 'Y')
        my_play = elf_val;
    else if(elf_val == 'A')
        my_play = my_val == 'X' ? 'C' : 'B';
    else if(elf_val == 'B')
        my_play = my_val == 'X' ? 'A' : 'C';
    else
        my_play = my_val == 'X' ? 'B' : 'A';
    
    return my_play - 'A' + 1 + battle_score;
}
