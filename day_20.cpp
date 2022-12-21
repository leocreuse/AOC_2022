#include <cmath>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

#define PART 2

#ifdef PART
    #define DECRYPT_KEY 811589153
#else
    #define DECRYPT_KEY 1
    #define PART 1
#endif

class Item;

void print_list(std::shared_ptr<Item> item);

class Item{
public:
    std::shared_ptr<Item> prev;
    std::shared_ptr<Item> next;
    int value;

    Item (int value){
        this->value = value;
        this->prev = nullptr;
        this->next = nullptr;
    }

    std::shared_ptr<Item> move(std::shared_ptr<Item> first, long long total){
        auto current_first = first;
        long long swap_count = 0;
        long long limit = std::abs ((long long) this->value * DECRYPT_KEY) % ((total - 1));
        while(swap_count < limit){
            if(this->value >= 0){
                auto current = this->next->prev;
                auto next = this->next;
                auto next_next = this->next->next;
                auto prev = this->prev;
                prev->next = next;
                next->prev = prev;
                next->next = current;
                current->prev = next;
                current->next = next_next;
                next_next->prev = current;
                if(next == current_first)
                    current_first = current;
                else if(current == current_first)
                    current_first = next;
            } else {
                auto current = this->next->prev;
                auto prev = this->prev;
                auto prev_prev = this->prev->prev;
                auto next = this->next;
                prev_prev->next = current;
                current->prev = prev_prev;
                current->next = prev;
                prev->prev = current;
                next->prev = prev;
                prev->next = next;
                if(prev == current_first)
                    current_first = current;
                else if(current == current_first)
                    current_first = prev;
            }
            swap_count++;
        }
        return current_first;
    }
};

std::shared_ptr<Item> append_new (std::shared_ptr<Item> current, int value){
    auto new_item = std::make_shared<Item>(value);
    new_item->prev = current;
    current->next = new_item;
    return new_item;
}

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    std::list<std::shared_ptr<Item>> input;
    std::shared_ptr<Item> zero;
    std::shared_ptr<Item> first;
    long long total_lines = 0;

    while (std::getline (input_file, line)){
        auto val = std::stoi(line);
        if(input.empty()){
            first = std::make_shared<Item>(val);
            input.push_back(first);
        }
        else
            input.push_back(append_new(input.back(), val));
        
        if(val == 0)
            zero = input.back();

        total_lines++;
    }

    std::cout << "Number of elements: " << total_lines << std::endl;

    // close the circular list
    input.front()->prev = input.back();
    input.back()->next = input.front();

    print_list(first);

    

    for(auto it = input.begin(); it != input.end(); it++){
        first = (*it)->move(first, total_lines);
    }

    // print_list(first);
    std::cout << "1" << std::endl;
    if(PART == 2)
        for(int i=0; i<9; i++){
            for(auto it = input.begin(); it != input.end(); it++){
                first = (*it)->move(first, total_lines);
            }
            // print_list(first);
            std::cout << i + 1 << std::endl;
        }

    std::shared_ptr<Item> current = zero;
    for(int i=0; i<1000; i++)
        current = current->next;
    std::shared_ptr<Item> one = current;
    for(int i=0; i<1000; i++)
        current = current->next;
    std::shared_ptr<Item> two = current;
    for(int i=0; i<1000; i++)
        current = current->next;
    std::shared_ptr<Item> three = current;
    std::cout << "1000: " << (long long) one->value * DECRYPT_KEY << std::endl;
    std::cout << "2000: " << (long long)two->value * DECRYPT_KEY<< std::endl;
    std::cout << "3000: " << (long long) three->value * DECRYPT_KEY << std::endl;
    std::cout << "Result: " << (long long) (one->value + two->value + three->value) * DECRYPT_KEY << std::endl;

}

void print_list(std::shared_ptr<Item> item){\
    auto curr = item;
    while(curr->next != item){
        std::cout << (long long) curr->value * DECRYPT_KEY << ", ";
        curr = curr->next;
    }
    std::cout << (long long) curr->value * DECRYPT_KEY;
    std::cout << std::endl;
}