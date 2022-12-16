#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>


#define PART_1_Y 2000000
// #define PART_1_Y 10

struct interval{
    long long min;
    long long max;
};

class Sensor {
public:
    long long x;
    long long y;
    long long bx;
    long long by;
    long long radius;

    Sensor (long long x, long long y, long long bx, long long by) {
        this->x = x;
        this->y = y;
        this->bx = bx;
        this->by = by;
        this->radius = std::abs(x - bx) + std::abs (y - by);
    }

    interval intersect_line (long long y){
        auto diff_y = std::abs(this->y - y);
        if(diff_y > this->radius)
            return {0, -1};
        
        return {this->x - (this->radius - diff_y), this->x + (this->radius - diff_y)};
    }

};

bool operator==(interval left, interval right){
    return left.min == right.min && left.max == right.max;
}

bool intersect (interval left, interval right){
    return left.min <= right.max && left.max >= right.min;
}

void add_interval (std::vector<interval> *list, interval new_elt){
    if(new_elt.max < new_elt.min)
        return;
    for(auto it = list->begin(); it!= list->end(); it++){
        if(intersect(*it, new_elt)){
            auto new_min = std::min (it->min, new_elt.min);
            auto new_max = std::max(it->max, new_elt.max);
            list->erase(it);
            add_interval(list, {new_min, new_max});
            return;
        }
    }
    list->push_back(new_elt);
}

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    std::vector<Sensor> sensors;
    std::vector<interval> part_1_int;

    while (std::getline (input_file, line)){
        size_t end;
        size_t start = 0;
        auto x = std::stoll(line, &end);
        start += end + 1;
        auto y = std::stoll (line.substr(start), &end);
        start += end + 1;
        auto bx = std::stoll(line.substr(start), &end);
        start += end + 1;
        auto by = std::stoll(line.substr(start));
        sensors.push_back(Sensor(x,y,bx,by));
        add_interval(&part_1_int, sensors.back().intersect_line(PART_1_Y));
    }

    auto part_1_score = 0;
    for(auto intv = part_1_int.begin(); intv != part_1_int.end(); intv++){
        part_1_score += intv->max - intv->min + 1;
    }

    // Only one beacon on line
    part_1_score--;

    std::cout << "Part 1: " << part_1_score << std::endl;

    std::vector<interval> lst;
    auto beacon_y=0;
    for(long long y = 0; y < 4000000; y++){
        if(y%100000 == 0)
            std::cout << " " << y;
        lst.clear();
        for(auto sensor = sensors.begin(); sensor != sensors.end(); sensor++){
            auto itv = sensor->intersect_line(y);
            if(itv.max >= 0 && itv.min <= 4000000)
                add_interval(&lst, itv);
        }
        if(lst.size() > 1){
            std::cout << std::endl << "[" << lst[0].min << ", " << lst[0].max << "] -> [" << lst[1].min << ", " << lst[1].max << "]" << std::endl;
            std::cout << lst.size();
            beacon_y = y;
            break;
        }
    }
    auto beacon_x = std::min (lst[0].max, lst[1].max) + 1;

    std::cout << std::endl << "Part 2: " << beacon_x << ", " << beacon_y << ", " << 4000000 * beacon_x + beacon_y <<std::endl;

}
