#include <cstdint>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <limits>

class Point{
public:
    int line;
    int col;
    int pred;
    int dist;
    bool visited;

    Point (int line, int col) {
        this->line = line;
        this->col = col;
        pred = -1;
        dist = std::numeric_limits<int>::max();
        visited = false;
    }

    void update_dist(Point *from, int idx){
        if(this->dist > from->dist + 1){
            this->dist = from->dist + 1;
            this->pred = idx;
        }
    }
};

int min (std::vector<Point> *points);

void dijkstra(std::vector<std::vector<int>> *map, std::vector<Point> *point);

void reset(std::vector<Point> * pts);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;
    std::vector<std::vector<int>> map;
    std::vector<Point> points;
    auto ln = 0;
    int start [2];
    std::vector<int> starts;
    int end [2];
    while(std::getline(input_file, line)){
        std::vector<int> cur_line;
        for (int col = 0; col < line.size(); col++){
            if(line[col] == 'S'){
                start[0] = ln;
                start[1] = col;
                cur_line.push_back(0);
            } else if(line[col] == 'E'){
                end[0] = ln;
                end[1] = col;
                cur_line.push_back(26);
            } else {
                cur_line.push_back(line[col] - 'a');
            }
            points.push_back(Point(ln,col));
            if(line[col] == 'S'){
                points.back().dist = 0;
            }

        }
        map.push_back(cur_line);
        ln++;
    }
    std::cout << "Start: (" << start[0] << "," << start[1] << ")" << std::endl;
    std::cout << "End  : (" << end[0] << "," << end[1] << ")" << std::endl;
    dijkstra(&map, &points);

    const auto end_idx = end[0] * map[0].size() + end[1];
    auto start_idx = start[0] * map[0].size() + start[1];
    std::cout << "Part 1: " << points[end_idx].dist << std::endl;

    for(int i=0; i<map.size(); i++){
        for(int j=0; j<map[0].size(); j++){
            if(map[i][j] == 0)
                starts.push_back(map[0].size() * i + j);
        }
    }
    auto part_2_res = map.size() * map[0].size();
    while(!starts.empty()){
        reset(&points);
        points[starts.back()].dist = 0;
        dijkstra(&map, &points);
        if(part_2_res > points[end_idx].dist){
            part_2_res = points[end_idx].dist;
        }
        starts.pop_back();
    }
    std::cout << "Part 2: " << part_2_res << std::endl;
    
}

int min (std::vector<Point> *points){
    auto min = std::numeric_limits<int>::max();
    auto min_idx = -1;
    int idx = 0;
    for(auto it = points->begin(); it != points->end(); it++){
        if(!it->visited && it->dist < min){
            min = it->dist;
            min_idx = idx;
        }
        idx++;
    }
    return min_idx;
}

void dijkstra(std::vector<std::vector<int>> *map, std::vector<Point> *points){
    auto idx = min(points);
    while(idx != -1){
        (*points)[idx].visited = true;
        const auto line = (*points)[idx].line;
        const auto col = (*points)[idx].col;
        
        // Up
        if(line != 0 && (*map)[line-1][col] <= (*map)[line][col] + 1){
            auto neigh_idx = (*map)[0].size() * (line - 1) + col;
            (*points)[neigh_idx].update_dist(&(*points)[idx], idx);
        }
        
        // Down
        if(line < map->size() - 1 && (*map)[line+1][col] <= (*map)[line][col] + 1){
            auto neigh_idx = (*map)[0].size() * (line + 1) + col;
            (*points)[neigh_idx].update_dist(&(*points)[idx], idx);
        }

        // Left
        if(col != 0 && (*map)[line][col - 1] <= (*map)[line][col] + 1){
            auto neigh_idx = (*map)[0].size() * line + col - 1;
            (*points)[neigh_idx].update_dist(&(*points)[idx], idx);
        }

        // Right
        if(col < (*map)[0].size() - 1 && (*map)[line][col + 1] <= (*map)[line][col] + 1){
            auto neigh_idx = (*map)[0].size() * line + col + 1;
            (*points)[neigh_idx].update_dist(&(*points)[idx], idx);
        }
        idx = min(points);
    }
}

void reset(std::vector<Point> *pts){
    for(auto pt=pts->begin(); pt != pts->end(); pt++){
        pt->dist = std::numeric_limits<int>::max();
        pt->visited = false;
    }
}