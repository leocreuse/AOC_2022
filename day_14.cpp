#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum Cell_State {Empty, Rock, Sand};

struct point_rec {
    int x;
    int y;
};

bool operator==(point_rec left, point_rec right){
    return left.x == right.x && left.y == right.y;
}

void pp_map(std::vector<std::vector<Cell_State>> *map);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    
    std::vector<std::vector<point_rec>> rocks;

    int min_x=500;
    int max_x=500;
    int max_y=0;

    while (std::getline (input_file, line)){
        size_t start = 0;
        rocks.push_back(std::vector<point_rec>());
        while(start < line.size()){
            size_t end;
            auto x = std::stoi(line.substr(start), &end);
            start += end + 1;
            auto y = std::stoi(line.substr(start), &end);
            start += end + 1;
            rocks.back().push_back({x,y});
            if(x < min_x)
                min_x = x;
            if(x > max_x)
                max_x = x;
            if(y > max_y)
                max_y = y;
            std::cout << "{" << x << ", " << y << "}" << std::endl;
        }
    }

    max_y+=3;
    min_x = (min_x < 500 - max_y ? min_x : 500 - max_y) + 1;
    max_x = (max_x > 500 + max_y ? max_x : 500 + max_y) + 1;

    const int origin = 500 - min_x;
    std::vector<std::vector<Cell_State>> map = std::vector<std::vector<Cell_State>>(max_y, std::vector<Cell_State>(max_x - min_x + 1, Empty));
    for(auto it = rocks.begin(); it != rocks.end(); it++){
        for(int i=1; i< it->size(); i++){
            if((*it)[i].x == (*it)[i-1].x){
                auto from = std::min((*it)[i].y, (*it)[i-1].y);
                auto to = std::max((*it)[i].y, (*it)[i-1].y);
                for(int y = from; y <= to; y ++)
                    map[y][(*it)[i].x - min_x] = Rock;
            } else {
                auto from = std::min((*it)[i].x, (*it)[i-1].x) - min_x;
                auto to = std::max((*it)[i].x, (*it)[i-1].x) - min_x;
                for(int x = from; x <= to; x ++)
                    map[(*it)[i].y][x] = Rock;
            }
        }
    }

    // Add floor
    for(int i=0; i<map[map.size()-1].size(); i++){
        map[map.size()-1][i] = Rock;
    }

    std::cout << map.size() << " " << map[0].size() << " " << min_x << " " << max_x << " " << max_y << std::endl;
    std::cout << "Empty map:" << std::endl;
    pp_map(&map);

    point_rec current = {origin, 0};
    point_rec prev = {origin, 0};
    auto first_move = true;
    auto sand_count = 0;
    auto part_1_score = 0;
    while(current.y != 0 || first_move){
        if(!first_move && current == prev){
            if(current.y == map.size() - 2 && part_1_score == 0){
                part_1_score = sand_count;
                std::cout << std::endl << "First hit:" << std::endl;
                pp_map(&map);
            }
            map[current.y][current.x] = Sand;
            sand_count +=1;
            current = {origin, 0};
            prev = {origin, 0};
            first_move = true;
            // pp_map(&map);
        } else {
            prev = current;
            if(map[current.y + 1][current.x] == Empty)
                current.y++;
            else if(map[current.y + 1][current.x - 1] == Empty){
                current.y++;
                current.x--;
            } else if(map[current.y + 1][current.x + 1] == Empty){
                current.y++;
                current.x++;
            }
            first_move = false;
        }
    }
    map[0][origin] = Sand;
    std::cout << std::endl<< "End map:" << std::endl;
    pp_map(&map);
    std::cout << "Part 1: " << part_1_score << std::endl;
    std::cout << "Part 2: " << sand_count + 1 << std::endl;
}

void pp_map(std::vector<std::vector<Cell_State>> *map){
    for(auto ln=map->begin(); ln != map->end(); ln++){
        for(auto cell = ln->begin(); cell != ln->end(); cell++){
            switch(*cell){
                case Empty:
                    std::cout << ".";
                    break;
                case Rock:
                    std::cout << "#";
                    break;
                case Sand:
                    std::cout << "o";
                    break;
            }
        }
        std::cout << std::endl;
    }
}