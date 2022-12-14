#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

void move_end (std::vector<char> *from, std::vector<char> *to, int count);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    std::vector<char> stacks1[9];
    std::vector<char> stacks2[9];
    std::vector<std::string> map;

    while(std::getline(input_file, line) && line != " 1   2   3   4   5   6   7   8   9 "){
        map.push_back(line);
    }
    while(!map.empty()){
        for(int i=0; i<9; i++){
            if(map.back()[1 + (4*i)] != ' ') {
                stacks1[i].push_back(map.back()[1 + 4*i]);
                stacks2[i].push_back(map.back()[1 + 4*i]);
            }
        }
        map.pop_back();
    }
    std::getline(input_file, line);
    
    while(std::getline(input_file, line)){
        std::string::size_type pos;
        auto amount = std::stoi(line.substr(5), &pos);
        auto from = std::stoi(line.substr(pos + 11)) - 1;
        auto to = std::stoi(line.substr(pos + 16)) - 1;
        for(int count = 0; count < amount; count ++){
            stacks1[to].push_back(stacks1[from].back());
            stacks1[from].pop_back();
        }
        move_end(&stacks2[from], &stacks2[to], amount);
    }
    std::string part_1_res = "";
    std::string part_2_res = "";
    for(int i=0; i< 9; i++){
        part_1_res += stacks1[i].back();
        part_2_res += stacks2[i].back();
    }
    std::cout << "Part 1: " << part_1_res << std::endl;
    std::cout << "Part 2: " << part_2_res << std::endl;
}


void move_end (std::vector<char> *from, std::vector<char> *to, int count){
    for(int i=count; i>0; i--){
        to->push_back(from->at(from->size() - i));
    }
    for(int i=0; i<count; i++){
        from->pop_back();
    }
}
