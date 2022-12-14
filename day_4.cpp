#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <set>
#include <algorithm>

bool contains(int l_min, int l_max, int r_min, int r_max);

bool overlaps(int l_min, int l_max, int r_min, int r_max);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    auto part_1_score = 0;
    auto part_2_score = 0;
    auto group_idx = 0;
    std::set<char> group_set;
    while(std::getline(input_file, line)){
       auto comma_pos = line.find(',');
       auto dash_1 = line.find('-');
       auto dash_2 = line.find('-', comma_pos + 1);
       auto l_min = std::stoi(line.substr(0, dash_1));
       auto l_max = std::stoi (line.substr(dash_1 + 1, comma_pos - dash_1 + 1));
       auto r_min = std::stoi(line.substr(comma_pos + 1, dash_2 - comma_pos + 1));
       auto r_max = std::stoi (line.substr(dash_2 + 1));
       part_1_score += contains(l_min, l_max, r_min, r_max) ? 1 : 0;
       part_2_score += overlaps(l_min, l_max, r_min, r_max) ? 1 : 0;
    }

    std:: cout << "Part 1: " << part_1_score << std::endl;
    std:: cout << "Part 2: " << part_2_score << std::endl;
}

bool contains(int l_min, int l_max, int r_min, int r_max){
    return (l_min <= r_min && l_max >= r_max) || (l_min >= r_min && l_max <= r_max);
}

bool overlaps(int l_min, int l_max, int r_min, int r_max){
    return !((l_max < r_min) || (l_min > r_max));
}