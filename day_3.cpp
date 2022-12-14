#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <set>
#include <algorithm>

int char_score (char chr);

int score_1 (std::string &line);

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
        // Part 1 processing
        part_1_score += score_1(line);

        // Part 2 processing
        if(group_idx % 3 == 0){
            // Set empty on first iteration
            if(!group_set.empty()){
                auto iter = group_set.begin();
                part_2_score += char_score(*iter);
            }
            group_set.clear();
            for(char &chr : line){
                group_set.insert(chr);
            }
        } else {
            std::set<char> line_set;
            std::set<char> res;
            for(char &chr : line){
                line_set.insert(chr);
            }
            std::set_intersection(group_set.begin(), group_set.end(), line_set.begin(), line_set.end(), std::inserter(res, res.begin()));
            group_set.swap(res);
        }
        group_idx++;
    }
    auto iter = group_set.begin();
    part_2_score += char_score(*iter);
    std:: cout << "Part 1: " << part_1_score << std::endl;
    std:: cout << "Part 2: " << part_2_score << std::endl;
}

int char_score (char chr){
    return  chr < 'a' ? chr - 'A' + 27 : chr - 'a' + 1;
}

int score_1 (std::string &line){
    auto mid_point = line.length() / 2;
    for(int idx = 0; idx < mid_point; idx++){
        auto chr = line[idx];
        if(line.find(chr, mid_point) != std::string::npos)
            return char_score(chr);
    }
    return 0;
}
