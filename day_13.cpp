#include <cstddef>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <utility>

enum Any_Kind {Int_Kind, List_Kind};

class Value;

std::shared_ptr<Value> parse(std::string *input, size_t start, size_t *end);

class Value{
public:
    Any_Kind kind;
    int int_value;
    std::vector<std::shared_ptr<Value>> list_value;

    Value (Any_Kind kind){
        this->kind = kind;
        this->int_value = 0;
        this->list_value.clear();
    }

    Value (Any_Kind kind, int int_value){
        this->kind = kind;
        this->int_value = int_value;
        this->list_value.clear();
    }

    Value as_list(){
        auto res = Value(List_Kind);
        res.list_value.push_back(std::make_shared<Value>(Int_Kind, this->int_value));
        return res;
    }

    bool operator< (Value other){
        if(this->kind == Int_Kind && other.kind == Int_Kind){
            return this->int_value < other.int_value;
        } else if (this->kind == List_Kind && other.kind == List_Kind) {
            if(this->list_value.empty() && other.list_value.empty())
                return false;
            else if(this->list_value.empty())
                return true;
            else if (other.list_value.empty())
                return false;
            for(int i = 0; i < this->list_value.size(); i++){
                if(i >= other.list_value.size())
                    return false;
                if(*this->list_value[i].get() < *other.list_value[i].get())
                    return true;
                if(*other.list_value[i].get() < *this->list_value[i].get())
                    return false;
            }
            return this->list_value.size() < other.list_value.size();
        } else if(this->kind == Int_Kind)
            return this->as_list() < other;
        else
            return *this < other.as_list();
    }
};

std::shared_ptr<Value> parse(std::string *input, size_t start, size_t *end){
    if(input->at(start) != '['){
        auto val = std::stoi(input->substr(start), end);
        *end += start;
        return std::make_shared<Value>(Int_Kind, val);
    }
    auto res = std::make_shared<Value>(List_Kind);
    if(input->at(start + 1) == ']'){
        *end = start + 2;
        return res;
    }
    res->list_value.push_back(parse(input, start + 1, end));
    while(input->at(*end) == ','){
        start = *end + 1;
        res->list_value.push_back(parse(input, start, end));
    }
    (*end)++;
    return res;
}

std::ostream &operator<<(std::ostream &os, const Value &val){
    if(val.kind == Int_Kind)
        os << val.int_value;
    else{
        os << "[";
        for(int i=0; i<val.list_value.size(); i++){
            os << *val.list_value[i].get();
            if(i != val.list_value.size() - 1)
                os << ",";
        }
        os << "]";
    }
    return os;
}

void p_val (Value val){
    std::cout << val << std::endl;
}

bool comp (std::shared_ptr<Value> left, std::shared_ptr<Value> right) {
    return *left < *right;
}

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    std::shared_ptr<Value> first;
    std::shared_ptr<Value> second;

    std::vector<std::shared_ptr<Value>> packets;

    auto index = 1;
    auto first_in_pair = true;

    auto part_1_score = 0;

    size_t dummy;

    while (std::getline (input_file, line)){
        if(line == ""){
            packets.push_back(first);
            packets.push_back(second);
            if(first != nullptr && second != nullptr && *first.get() < *second.get()){
                part_1_score += index;
            }
            index ++;
        } else if(first_in_pair){
            first = parse(&line, 0, &dummy);
            first_in_pair = false;
        } else {
            second = parse(&line, 0, &dummy);
            first_in_pair = true;
        }
    }

    std::cout << "Part 1: " << part_1_score << std::endl;

    std::string start_pkt = "[[2]]";
    std::string end_pkt = "[[6]]";
    auto start = parse(&start_pkt, 0, &dummy);
    auto end = parse(&end_pkt, 0, &dummy);
    packets.push_back(start);
    packets.push_back(end);
    std::sort(packets.begin(), packets.end(), &comp);
    index = 1;
    auto part_2_score = 1;
    for(auto it = packets.begin(); it !=packets.end(); it++){
        if(*it == start || *it == end)
            part_2_score *= index;
        index++;
    }
    std::cout << "Part 2: " << part_2_score << std::endl;
}