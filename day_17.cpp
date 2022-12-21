#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

const std::vector<std::vector<std::string>> shapes ={
    {
        "#..@@@@.#"
    },
    {
        "#...@...#",
        "#..@@@..#",
        "#...@...#"
    },
    {
        "#..@@@..#",
        "#....@..#",
        "#....@..#"
    },
    {
        "#..@....#",
        "#..@....#",
        "#..@....#",
        "#..@....#"
    },
    {
        "#..@@...#",
        "#..@@...#"
    }
};

bool can_move (std::vector<std::string> *map, std::vector<long long> *lines, char move);
void do_move (std::vector<std::string> *map, std::vector<long long> *lines, char move);
long long rockify (std::vector<std::string> *map, std::vector<long long> *lines, long long *bot);
void spawn(std::vector<std::string> *map, std::vector<long long> *lines, int shape, long long top);
void print_map(std::vector<std::string> *map);
void stringify (std::vector<std::string> *map);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string input;
    std::getline(input_file, input);

    std::vector<std::string> cave;
    cave.push_back("#########");
    long long top = 1;
    long long nb_rocks = 0;
    int current_shape = 1;
    bool move_down = false;
    long long current_push = 0;
    long long bot = 0;
    /*
    offset_height: 349
    offset_block: 147
    loop_height: 2783
    loop_blocks: 1745
    start instruction: 1639
    missing blocks : 270
    remaining height: 1576
    first loop rock = 4;
    */
    std::vector<long long> lines;
    spawn(&cave, &lines, 4, 0);
    current_push = 1639;
    while(nb_rocks < 1010){
        if (!move_down){
            if(can_move(&cave, &lines, input[current_push % input.size()]))
                do_move(&cave, &lines, input[current_push % input.size()]);
            current_push++;
            move_down = true;
        } else {
            if(can_move(&cave, &lines, 'v'))
                do_move(&cave, &lines, 'v');
            else{
                top = rockify(&cave, &lines, &bot);
                spawn(&cave, &lines, current_shape, top);
                
                current_shape++;
                current_shape %= 5;
                nb_rocks++;
            }
            move_down = false;
            if(nb_rocks == 2022){
                //print_map(&cave);
                std::cout << "Part 1: " << top + bot << std::endl;
            }
        }
    }
    // print_map(&cave);
    // stringify(&cave);
    
}

bool can_move (std::vector<std::string> *map, std::vector<long long> *lines, char move){
    switch(move){
        case 'v':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = 0; i<map->at(*ln).size(); i++){
                    if(map->at(*ln)[i] == '@' && map->at((*ln) -1)[i] == '#')
                        return false;
                }
            }
            break;
        case '>':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = 0; i<map->at(*ln).size() - 1; i++){
                    if(map->at(*ln)[i] == '@' && map->at(*ln)[i+1] == '#')
                        return false;
                }
            }
            break;
        case '<':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = 1; i<map->at(*ln).size(); i++){
                    if(map->at(*ln)[i] == '@' && map->at(*ln)[i-1] == '#')
                        return false;
                }
            }
            break;
    }
    return true;
}

void do_move (std::vector<std::string> *map, std::vector<long long> *lines, char move){
    switch(move){
        case 'v':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = 0; i<map->at(*ln).size(); i++){
                    if(map->at(*ln)[i] == '@'){
                        map->at((*ln) -1)[i] = '@';
                        map->at(*ln)[i] = '.';
                    }
                }
                (*ln)--;
            }
            break;
        case '<':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = 1; i<map->at(*ln).size(); i++){
                    if(map->at(*ln)[i] == '@'){
                        map->at(*ln)[i-1] = '@';
                        map->at(*ln)[i] = '.';
                    }
                }
            }
            break;
        case '>':
            for(auto ln = lines->begin(); ln != lines->end(); ln++){
                for(long long i = map->at(*ln).size() - 1; i >= 0; i--){
                    if(map->at(*ln)[i] == '@'){
                        map->at(*ln)[i+1] = '@';
                        map->at(*ln)[i] = '.';
                    }
                }
            }
            break;
    }
}

long long rockify (std::vector<std::string> *map, std::vector<long long> *lines, long long *bot){
    for(auto i : *lines){
        for(long long j = 0; j < map->at(i).size(); j++){
            if(map->at(i)[j] == '@')
                map->at(i)[j] ='#';
        }
    }
    // for(long long i=1; i < map->size(); i++){
    //     if(map->at(i).find_first_of("@.") == std::string::npos){
    //         map->erase(map->begin(), map->begin() + i);
    //         (*bot) += i;
    //         break;
    //     }
    // }
    for(long long i=map->size() - 1; i > 0; i--){
        for(long long j=1; j<map->at(i).size() - 1; j++){
            if(map->at(i)[j] == '#')
                return i;
        }
    }
    return 0;
}


void spawn(std::vector<std::string> *map, std::vector<long long> *lines, int shape, long long top){
    while(map->size() > top + 1)
        map->pop_back();
    
    map->push_back("#.......#");
    map->push_back("#.......#");
    map->push_back("#.......#");

    lines->clear();

    for(auto line : shapes[shape%5]){
        map->push_back(line);
        lines->push_back(map->size() - 1);
    }
}

void print_map(std::vector<std::string> *map){
    for(auto it=map->rbegin(); it !=map->rend(); ++it){
        std::cout << *it << std::endl;
    }
}

void stringify (std::vector<std::string> *map){
    for(auto it = map->begin(); it != map->end(); it++){
        unsigned int val = 0;
        for(int idx=1; idx < it->size() - 1; idx++){
            if(it->at(idx) == '#'){
                val |= 1 << (idx -1);
            }
        }
        std::cout << val << ",";
    }
}