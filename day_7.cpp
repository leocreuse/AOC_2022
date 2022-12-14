#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

#define FILE_LIMIT 100000
#define TOTAL_SIZE 70000000
#define REQ_FREE   30000000

class Dir {
    public:
    Dir *parent;
    std::string name;
    int file_size;
    int total_size;
    std::vector<Dir *> subdirs;

    Dir(Dir *parent, std::string name){
        this->name = name;
        this->parent = parent;
        this->file_size = 0;
        this->total_size = 0;
    }
};

int count_small (Dir* root);

int find_delete_dir (Dir *root, int min_size);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    Dir *current_dir = NULL;
    Dir * root_dir = NULL;
    while(std::getline(input_file, line)){
        if(line == "$ cd .."){
            current_dir = current_dir->parent;
        } else if(line.substr(0, 4) == "$ cd"){
            Dir* new_dir = new Dir(current_dir, line.substr(4));
            if(current_dir != NULL)
                current_dir->subdirs.push_back(new_dir);
            else
                root_dir = new_dir;
            current_dir = new_dir;
        } else if (line[0] >= '0' && line[0] <= '9') {
            current_dir->file_size += std::stoi(line);
        }
    }
    auto part_1_res = count_small(root_dir);
    std::cout << "Part 1: " << part_1_res << std::endl;
    auto missing_free = REQ_FREE - TOTAL_SIZE + root_dir->total_size;
    auto part_2_res = find_delete_dir(root_dir, missing_free);
    std::cout << "Part 2: " << part_2_res << std::endl;

}

int count_small(Dir *root){
    auto sum = 0;
    root->total_size = root->file_size;
    for(auto subdir : root->subdirs){
        sum += count_small(subdir);
        root->total_size += subdir->total_size;
    }
    if(root->total_size <= FILE_LIMIT)
        return sum + root->total_size;
    else
        return sum;
}

int find_delete_dir (Dir *root, int min_size){
    auto current_min = TOTAL_SIZE;
    for(auto subdir : root->subdirs){
        if(subdir->total_size < min_size)
            continue;
        auto subdir_res = find_delete_dir(subdir, min_size);
        if(subdir_res < current_min)
            current_min = subdir_res;
    }
    if(current_min == TOTAL_SIZE && root->total_size > min_size)
        current_min = root->total_size;
    return current_min;
}