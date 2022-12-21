#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define CUBE_SIZE 22

enum Cell_State {Rock, Out, In};

int count_faces(std::vector<std::vector<std::vector<Cell_State>>> *map, Cell_State what);
void mark_out(std::vector<std::vector<std::vector<Cell_State>>> *map);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    std::vector<std::vector<std::vector<Cell_State>>> rocks(CUBE_SIZE, std::vector<std::vector<Cell_State>>(CUBE_SIZE, std::vector<Cell_State>(CUBE_SIZE, In)));
    while (std::getline (input_file, line)){
        size_t end = 0;
        int start = 0;
        int x = std::stoi(line, &end);
        start = end + 1;
        int y = std::stoi(line.substr(start), &end);
        start += end + 1;
        int z = std::stoi(line.substr(start));
        rocks[x][y][z] = Rock;
    }
    auto all_rock_faces = count_faces(&rocks, Rock);
    std::cout << "Part 1: " << all_rock_faces << std::endl;
    mark_out(&rocks);
    std::cout << "Part 2: " << all_rock_faces - count_faces(&rocks, In) << std::endl;
}

int count_faces(std::vector<std::vector<std::vector<Cell_State>>> *map, Cell_State what){
    auto res = 0;
    for(int x = 0; x < map->size(); x++){
        for(int y = 0; y < map->at(0).size(); y++){
            for (int z = 0; z < map->at(0)[0].size(); z++){
                if(x!= 0)
                    res += map->at(x)[y][z] == what && !(map->at(x-1)[y][z] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
                if(x!= map->size() - 1)
                    res += map->at(x)[y][z] == what && !(map->at(x+1)[y][z] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
                if(y!= 0)
                    res += map->at(x)[y][z] == what && !(map->at(x)[y-1][z] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
                if(y!= map->at(0).size() - 1)
                    res += map->at(x)[y][z] == what && !(map->at(x)[y+1][z] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
                if(z!= 0)
                    res += map->at(x)[y][z] == what && !(map->at(x)[y][z-1] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
                if(z!= map->at(0)[0].size() - 1)
                    res += map->at(x)[y][z] == what && !(map->at(x)[y][z+1] == what) ? 1 : 0;
                else
                    res += map->at(x)[y][z] == what ? 1 : 0;
            }
        }
    }
    return res;
}

void mark_out(std::vector<std::vector<std::vector<Cell_State>>> *map){
    auto out_count = 0;
    auto prev = out_count - 1;
    while(out_count != prev){
        prev = out_count;
        for(int x = 0; x<map->size(); x++){
            for(int y = 0; y<map->at(0).size(); y++){
                for(int z = 0; z<map->at(0)[0].size(); z++){
                    if(map->at(x)[y][z] != In)
                        continue;
                    
                    if(x!= 0 && (*map)[x-1][y][z] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(x==0){
                        out_count ++;
                        (*map)[x][y][z] = Out;
                    }
                    if(x != map->size() -1 && (*map)[x+1][y][z] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(x==map->size() - 1){
                        out_count ++;
                        (*map)[x][y][z] = Out;
                    }
                    if(y!= 0 && (*map)[x][y-1][z] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(y==0){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    if(y != (*map)[0].size() -1 && (*map)[x][y+1][z] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(y== (*map)[0].size() - 1){
                        out_count ++;
                        (*map)[x][y][z] = Out;
                    }
                    if(z!= 0 && (*map)[x][y][z-1] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(z==0){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    if(z!= map->size() -1 && (*map)[x][y][z+1] == Out){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                    else if(z==(*map)[0][0].size() - 1){
                        out_count++;
                        (*map)[x][y][z] = Out;
                    }
                }
            }
        }
    }
}