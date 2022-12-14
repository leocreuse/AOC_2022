#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    int cycle = 0;
    int x = 1;
    int next_x = 0;
    int next_update = 0;

    int part_one_score = 0;

    std::vector<std::string> screen;
    for(int i = 0; i < 6; i++){
        screen.push_back("........................................");
    }

    while(cycle <= 240 && !input_file.eof()){
        if(cycle == next_update){
            x += next_x;
            std::getline(input_file, line);
            if(line == "noop"){
                next_x = 0;
                next_update = cycle + 1;
            } else {
                next_x = std::stoi(line.substr(5));
                next_update = cycle + 2;
            }
        }
        cycle +=1;
        int current_line = cycle / 40;

        if(current_line > 5){
            break;
        }

        if(std::abs (((cycle - 1) % 40) - x) <= 1)
            screen[current_line][(cycle - 1)%40] = '#';

        if((cycle + 20) % 40 == 0){
            part_one_score += x * cycle;
            std::cout << "x: " << x << "cycle: " << cycle << std::endl;
        }
        
    }
    std::cout << "Part 1: " << part_one_score << std::endl;
    std::cout << "Part 2:" << std::endl;
    for(auto line : screen){
        std::cout << line << std::endl;
    }
}