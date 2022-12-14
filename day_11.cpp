#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <list>

#define ROUND_COUNT 10000
#define TEST false

class Monkey{
    public:
    long long index;
    std::list<long long> items;
    long long (*operation) (long long old);
    long long test_div;
    long long throw_true;
    long long throw_false;
    long long inspect_count;

    Monkey(long long index, std::list<long long> init_items, long long (*operation) (long long old), long long test_div, long long throw_true, long long throw_false){
        this->index = index;
        this->items = init_items;
        this->operation = operation;
        this->test_div = test_div;
        this->throw_true = throw_true;
        this->throw_false = throw_false;
        this->inspect_count = 0;
    }

    bool inspect_and_throw(std::vector<Monkey> *others){
        if(this->items.empty())
            return false;
        
        inspect_count +=1;
        auto current_worry = this->items.front();
        this->items.pop_front();
        auto md = 1;
        for(auto it = others->begin(); it != others->end(); it++){
            md *= it->test_div;
        }
        current_worry = this->operation (current_worry) % md;
        if(current_worry % this->test_div == 0){
            (*others)[this->throw_true].items.push_back(current_worry);
        } else {
            (*others)[this->throw_false].items.push_back(current_worry);
        }
        return true;
    }
};

long long count_items(std::vector<Monkey> *monkeys);

int main(int argc , char** argv) {
    
    std::vector<Monkey> test = {
        Monkey(0, {79, 98}, [](long long old) {return (19 * old);}, 23, 2, 3),
        Monkey(1, {54, 65, 75, 74}, [](long long old) {return (6 + old);}, 19, 2, 0),
        Monkey(2, {79, 60, 97}, [](long long old) {return (old * old);}, 13, 1, 3),
        Monkey(3, {74}, [](long long old) {return (old + 3);}, 17, 0, 1)
    };

    std::vector<Monkey> input = {
        Monkey(0, {62, 92, 50, 63, 62, 93, 73, 50}, [](long long old) {return 7 * old;}, 2, 7, 1),
        Monkey(1, {51, 97, 74, 84, 99}, [](long long old) {return 3 + old;}, 7, 2, 4),
        Monkey(2, {98, 86, 62, 76, 51, 81, 95}, [](long long old) {return 4 + old;}, 13, 5, 4),
        Monkey(3, {53, 95, 50, 85, 83, 72}, [](long long old) {return 5 + old;}, 19, 6, 0),
        Monkey(4, {59, 60, 63, 71}, [](long long old) {return 5 * old;}, 11, 5, 3),
        Monkey(5, {92, 65}, [](long long old) {return old * old;}, 5, 6, 3),
        Monkey(6, {78}, [](long long old) {return old + 8;}, 3, 0, 7),
        Monkey(7, {84, 93, 54}, [](long long old) {return old + 1;}, 17, 2, 1),
    };

    auto monkeys = TEST ? &test : &input;

    for(long long i=0; i<ROUND_COUNT; i++){
        // std::cout<< "Beginning round" << i + 1 << " with " << count_items(monkeys) << " items." << std::endl;
        for(auto it = monkeys->begin(); it != monkeys->end(); it++){
            while(it->inspect_and_throw(monkeys)){;}
        }
    }
    auto max1 = 0;
    auto max2 = 0;
    for(auto it = monkeys->begin(); it != monkeys->end(); it++){
        std::cout << it->inspect_count << std::endl;
        if(it->inspect_count > max1){
            max2 = max1;
            max1 = it->inspect_count;
        } else if (it->inspect_count > max2){
            max2 = it->inspect_count;
        }
    }
    std::cout << "total items: " << count_items(monkeys) << std::endl;
    std::cout << "Monkey business: " << max1 * max2 << std::endl;
    return 0;

}

long long count_items(std::vector<Monkey> *monkeys){
    auto count = 0;
    for(auto monkey = monkeys->begin(); monkey != monkeys->end(); monkey++){
        count += monkey->items.size();
    }
    return count;
}