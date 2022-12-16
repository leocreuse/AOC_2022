#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include<bitset>
#include <list>

using namespace std;
class Valve{
    public:
        string name;
        int flow_rate;
        vector<string> neigh_names;
        vector<Valve*> neighbors;
        bool opened;
        int distance;

        Valve(string name, int flow_rate){
            this->name = name;
            this->flow_rate = flow_rate;
            this->neigh_names = {};
            this->neighbors = {};
            this->opened = false;
            this->distance = -1;
        }

        Valve(Valve& v){
            this->name = v.name;
            this->flow_rate = v.flow_rate;
            this->neigh_names = {};
            this->neighbors = {};
            this->opened = false;
        }

        void print(){
            cout << this->name << ", " << this->flow_rate << ", " << this->opened << endl;
            cout << "Neighbors : ";
            for (auto it=(this->neighbors).begin(); it!=(this->neighbors).end(); it++){
                 cout << (*it)->name << ", ";
            }
            cout << endl;
        }
};

long long find_max_released_2(vector<Valve*> to_open, vector<Valve*> valves, Valve *current, int time_left, int current_flow, long long released_presure);
void reset_graph(vector<Valve*> valves);
int find_shortest_path(Valve *current, Valve *next);
int split_valves(vector<Valve*> to_open, vector<Valve *> valves, Valve * start);

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    Valve *start;
    vector<Valve*> valves = {};
    vector<Valve*> to_open = {};

    while (std::getline (input_file, line)){
        string name = line.substr(0, 2);
        string::size_type sz;
        int curr_idx = 0;
        int flow_rate = stoi(line.substr(3), &sz);
        Valve *v = new Valve(name, flow_rate);
        valves.push_back(v);
        curr_idx += 3 + sz + 1;
        vector<string> neigh_names = {};
        while (curr_idx < line.size()){
            v->neigh_names.push_back(line.substr(curr_idx, 2));
            curr_idx += 3;
        }

        if (name=="AA"){
            start = v;
        }
    }

    // populate the neighbors
    for (auto it=valves.begin(); it!=valves.end(); it++){
        for (string neigh_name : (*it)->neigh_names){
            for (int i=0; i<valves.size(); i++){
                if (valves[i]->name == neigh_name){
                    ((*it)->neighbors).push_back(valves[i]);
                }
            }
        }
        if ((*it)->flow_rate >0){
            to_open.push_back(*it);
        }
    }
    for (auto it=valves.begin(); it!=valves.end(); it++){
        (*it)->print();
    }

    auto part_1_score = find_max_released_2(to_open, valves, start, 30, 0, 0);
    std::cout << "Part 1: " << part_1_score << std::endl;
    auto part_2_score = split_valves(to_open, valves, start);
    std::cout << "Part 2: " << part_2_score << std::endl;

}

int split_valves(vector<Valve*> to_open, vector<Valve *> valves, Valve * start){
    int max = 0;
    int mid_size = to_open.size()/2;
    unsigned int mask = 0xFFFFFFFF << (to_open.size());
    for(unsigned int i=0; i < 1<<to_open.size(); i++){
        long long bits = bitset<32>(i|mask).count() - 32 + to_open.size(); 
        if(bits >= mid_size + 1 || bits <= mid_size - 1)
            continue;
        if(i % 1000 ==0)
            cout << i << endl;
        vector<Valve *> my_to_open;
        vector<Valve *> elephant_to_open;
        for(int j=0; j<to_open.size(); j++){
            if(i & 1<<j)
                my_to_open.push_back(to_open[j]);
            else
                elephant_to_open.push_back(to_open[j]);
        }
        int me_score = find_max_released_2(my_to_open, valves, start, 26, 0, 0);
        int elephant_score = find_max_released_2(elephant_to_open, valves, start, 26, 0, 0);
        if(me_score + elephant_score > max)
            max = me_score + elephant_score;
    }
    return max;
}

long long find_max_released_2(vector<Valve*> to_open, 
                              vector<Valve*> valves, 
                              Valve *current, 
                              int time_left, 
                              int current_flow, 
                              long long released_presure){
    long long curr_max = 0;
    if (time_left<1){
        return released_presure;
    }
    if (to_open.size()==0){
        return released_presure + current_flow*time_left;
    }
    for (auto next=to_open.begin(); next!=to_open.end(); next++){
        reset_graph(valves);
        int dist = find_shortest_path(current, *next);
        if (time_left>dist+1){
            vector<Valve*> new_to_open(to_open.size()-1);
            remove_copy(to_open.begin(), to_open.end(), new_to_open.begin(), *next);
            int curr_released = find_max_released_2(new_to_open, 
                                                valves, 
                                                *next, 
                                                time_left-dist-1, 
                                                current_flow + (*next)->flow_rate,
                                                released_presure + current_flow*(dist+1));
            if (curr_released > curr_max){
                curr_max = curr_released;
            }
        }    
    }
    if (curr_max==0){
        curr_max = current_flow*time_left+released_presure;
    }
    return curr_max;
}

void reset_graph(vector<Valve*> valves){
    for (Valve *v : valves){
        v->distance = -1;
    }
}

int find_shortest_path(Valve *current, Valve *next){
    list<Valve*> queue ={};
    current->distance = 0;
    queue.push_back(current);
    while (queue.size()>0){
        Valve *v = queue.front();
        queue.pop_front();
        if (v->name == next->name){
            return v->distance;
        }
        for (Valve *neigh : v->neighbors){
            if (neigh->distance == -1){
                neigh->distance = v->distance + 1;
                queue.push_back(neigh);
            }
        }
    }
    return 0;
}