#include <cstddef>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <math.h>

#define TOTAL_MINS 23

#define BUILD_RBT 1
#define WAIT_1 0
#define SKIP_RBT -1

enum Robot {Ore=0, Clay, Obsidian, Geode, Robot_Count};

const char* robot_names[Robot_Count] = {"Ore  ", "Clay ", "Obsi.", "Geode"};

class Factory{
public:
    int blueprint_num;
    int costs[Robot_Count][Robot_Count];
    int robot_stock[Robot_Count];
    int mineral_stock[Robot_Count];
    int state; // -1 -> do nothing, otherwise, producing robot x

    Factory(int blueprint_num ,int ore_ore, int clay_ore, int obs_ore, int obs_clay, int geode_ore, int geode_obs){
        this->blueprint_num = blueprint_num;
        for(int rb = Ore; rb < Robot_Count; rb++){
            for(int mat=Ore; mat < Robot_Count; mat++){
                this->costs[rb][mat] = 0;
            }
            this->robot_stock[rb] = 0;
            this->mineral_stock[rb] = 0;
        }
        this->costs[Ore][Ore] = ore_ore;
        this->costs[Clay][Ore] = clay_ore;
        this->costs[Obsidian][Ore] = obs_ore;
        this->costs[Obsidian][Clay] = obs_clay;
        this->costs[Geode][Ore] = geode_ore;
        this->costs[Geode][Obsidian] = geode_obs;

        this->robot_stock[Ore] = 1;
        this->state = -1;
    }

    Factory (Factory *other){
        this->blueprint_num = other->blueprint_num;
        this->state = other->state;
        for(int mat = Ore; mat < Robot_Count; mat++){
            for(int rb = Ore; rb < Robot_Count; rb++)
                this->costs[mat][rb] = other->costs[mat][rb];

            this->robot_stock[mat] = other->robot_stock[mat];
            this->mineral_stock[mat] = other->mineral_stock[mat];
        }
    }

    bool operator <(Factory other){
        int total_this[Robot_Count] = {0,0,0,0};
        int total_other[Robot_Count] = {0,0,0,0};
        for(int mat = Geode; mat >= Ore; mat--){
            // total_this[mat] += this->mineral_stock[mat];
            // total_other[mat] += other->mineral_stock[mat];
            // for(int rb = Ore; rb < Robot_Count; rb ++){
            //     total_this[mat] += this->robot_stock[mat] * this->costs[rb][mat];
            //     total_other[mat] += other->robot_stock[mat] * other->costs[rb][mat];
            // }
            // if(total_this[mat] == total_other[mat])
            //     continue;
            // return total_this[mat] < total_other[mat];
            // if (this->mineral_stock[mat] == other->mineral_stock[mat]){
            //     if(this->robot_stock[mat] == other->robot_stock[mat])
            //         continue;
            //     else
            //         return this->robot_stock[mat] < other->robot_stock[mat];
            // } else
            //     return this->mineral_stock[mat] < other->mineral_stock[mat];
        }
        return this->mineral_stock[Geode] < other.mineral_stock[Geode];
        return false;
    }

    void print(){
        std::printf("bp:%2d|Rbt|Stk|\n", this->blueprint_num);
        std::printf("=====+===+===+\n");
        for(int rb = Ore; rb < Robot_Count; rb++){
            std::printf("%s|%3d|%3d|\n",robot_names[rb], this->robot_stock[rb], this->mineral_stock[rb]);
            std::printf("-----+---+---+\n");
        }
        if(this->state == -1)
            std::printf("Not building anything..\n\n");
        else
            std::printf("Building %s robot\n\n", robot_names[this->state]);
    }

    bool can_build(int what){
        for(int mat = Ore; mat< Robot_Count; mat++){
            if(this->mineral_stock[mat] < this->costs[what][mat]){
                return false;
            }
        }
        return true;
    }

    int turns_to_build(int what){
        int min_turns = 0;
        for(int mat = Ore; mat < Robot_Count; mat++){
            if(this->costs[what][mat] != 0 && this->robot_stock[mat] == 0)
                return std::numeric_limits<int>::max();
            auto steps_left = std::ceil ((float)(this->costs[what][mat] - this->mineral_stock[mat]) / ((float) this->robot_stock[mat]));
            if(min_turns < steps_left){
                min_turns = steps_left;
            }
        }
        return min_turns;
    }

    void build(int what){
        for(int mat=Ore; mat < Robot_Count; mat++)
            this->mineral_stock[mat] -= this->costs[what][mat];
        this->state = what;
    }

    void step(){
        // Finish building a robot
        if(this->state != -1)
            this->robot_stock[state]++;
        this->state = -1;

        // Pick a robot to build

        int best_build = Ore;
        for(int mat = Ore; mat < Robot_Count; mat++){
            if(this->robot_stock[mat] != 0)
                best_build = mat;
        }

        for(int mat = Geode; mat >= best_build; mat--){
            auto ttb = this->turns_to_build(mat);
            if(ttb <= 2){
                if(ttb <= 0)
                    this->build(mat);
                break;
            }
        }

        // Harvest some materials :)
        for(int mat = Ore; mat < Robot_Count; mat++){
            this->mineral_stock[mat] += this->robot_stock[mat];
        }
    }

    std::shared_ptr<Factory> optimize_prod (int time_left){
        // Harvest some materials :)
        for(int mat = Ore; mat < Robot_Count; mat++){
            this->mineral_stock[mat] += this->robot_stock[mat];
        }
        // Finish building a robot
        if(this->state != -1)
            this->robot_stock[state]++;
        this->state = -1;
        if(time_left == 0)
            return std::make_shared<Factory>(this);


        auto max_ore_cost = 0;
        for(int mat=Ore; mat < Robot_Count; mat++)
            if(max_ore_cost < this->costs[mat][Ore])
                max_ore_cost = this->costs[mat][Ore];

        // Try out all other production possibilities
        std::shared_ptr<Factory> best = nullptr;
        for(int mat = Geode; mat >= Ore; mat--){
            //  Only build Ore robots in the early stages
            if(mat == Ore && time_left <= TOTAL_MINS - 3 * max_ore_cost){
                break;
            }

            if (this->can_build(mat)){
                auto new_fact = std::make_shared<Factory>(this);
                new_fact->build(mat);   
                auto build_res = new_fact->optimize_prod(time_left - 1);
                if(best == nullptr || *best < *build_res)
                    best = build_res;
            }
        }

        // Try not to build anything
        auto nothing = std::make_shared<Factory>(this);
        auto build_res = nothing->optimize_prod(time_left - 1);
        if(best == nullptr || *best < *nothing)
            best = build_res;

        return best;
    }

};

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    std::vector<std::shared_ptr<Factory>> factories;

    while (std::getline (input_file, line)){
        size_t start = 0;
        size_t end = 0;
        int num = std::stoi(line, &end);
        start += end + 1;
        auto ore_ore = std::stoi(line.substr(start), &end);
        start += end + 1;
        auto clay_ore = std::stoi(line.substr(start), &end);
        start += end + 1;
        auto obs_ore = std::stoi(line.substr(start), &end);
        start += end + 1;
        auto obs_clay = std::stoi(line.substr(start), &end);
        start += end + 1;
        auto geode_ore = std::stoi(line.substr(start), &end);
        start += end + 1;
        auto geode_obs = std::stoi(line.substr(start));
        factories.push_back(std::make_shared<Factory>(num, ore_ore, clay_ore, obs_ore, obs_clay, geode_ore, geode_obs));
    }
    for(auto fact = factories.begin(); fact != factories.end(); fact++){
        (*fact)->print();
    }

    std::vector<std::shared_ptr<Factory>> fact_prods;

    for(auto fact = factories.begin(); fact != factories.end(); fact++)
        fact_prods.push_back((*fact)->optimize_prod(TOTAL_MINS));
    auto part_1_res = 0;

    for(int i=0; i < fact_prods.size(); i++){
        std::cout << "Factory " << i+1 << " can produce " << fact_prods[i]->mineral_stock[Geode] << " geodes." << std::endl;
        part_1_res += (i+1) * fact_prods[i]->mineral_stock[Geode];
    }
    std::cout << "Part 1: " << part_1_res << std::endl;

    std::vector<std::shared_ptr<Factory>> part_2_facts;
    auto part_2_res = 1;
    for(int i=0; i<2; i++){
        fact_prods[i]->print();
        part_2_facts.push_back(fact_prods[i]->optimize_prod(8));
        std::cout << "Factory " << i+1 << " can produce " << part_2_facts[i]->mineral_stock[Geode] << " geodes." << std::endl;
        part_2_res *= part_2_facts[i]->mineral_stock[Geode];
    }
    std::cout << "Part 2: " << part_2_res << std::endl;

}
