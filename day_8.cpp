#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
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

    std::vector<std::vector<int>> map;

    while(std::getline(input_file, line)){
        std::vector<int> ln;
        for(auto c : line){
            ln.push_back(c - '0');
        }
        map.push_back(ln);
    }
    auto visible_count = 2 * map.size() + 2 * map[0].size() - 4;
    for(int i=1; i<map.size() - 1; i++){
        for(int j=1; j<map[i].size() - 1; j++){
            bool visible = true;
            for(int k=0; k < map.size(); k++){
                if(k < i && map[k][j] >= map[i][j])
                    k = i;
                else if(k == i){
                    break;
                } else if(map[k][j] >= map[i][j]){
                    visible = false;
                }
            }
            if(visible){
                visible_count +=1;
                continue;
            }
            visible = true;
            for(int k=0; k < map[0].size(); k++){
                if(k < j && map[i][k] >= map[i][j])
                    k = j;
                else if(k == j){
                    break;
                } else if(map[i][k] >= map[i][j]){
                    visible = false;
                }
            }
            if(visible){
                visible_count +=1;
            }
        }
    }
    std::cout << "Part 1: " << visible_count << std::endl;
    auto best_scenic_score = 0;
    for(int i=1; i<map.size() - 1; i++){
        for(int j=1; j<map[i].size() - 1; j++){
            std::cout << i << " " << j << " ";
            auto score_left = i;
            for (int k = i-1; k >= 0; k--){
                if(map[k][j] >= map[i][j]){
                    score_left = i - k;
                    break;
                }
            }

            auto score_right = map.size() - i - 1;
            for (int k = i + 1; k < map.size(); k++){
                if(map[k][j] >= map[i][j]){
                    score_right = k - i;
                    break;
                }
            }

            auto score_up = j;
            for (int k = j-1; k >= 0; k--){
                if(map[i][k] >= map[i][j]){
                    score_up = j - k;
                    break;
                }
            }

            auto score_down = map[0].size() - j - 1;
            for (int k = j + 1; k < map.size(); k++){
                if(map[i][k] >= map[i][j]){
                    score_down = k - j;
                    break;
                }
            }
            std::cout << score_left << " " << score_right << " " << score_up << " " << score_down << std::endl;
            if (score_left * score_right * score_up * score_down > best_scenic_score)
                best_scenic_score = score_left * score_right * score_up * score_down;
        }
    }
    std::cout << "Part 2: " << best_scenic_score << std::endl;

}
