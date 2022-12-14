#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <set>
#include <vector>

class  Point{
public:
    int x;
    int y;
public:
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator < (Point const other) const{
        if(this->x == other.x)
            return this->y < other.y;
        return this->x < other.x;
    }
    void follow(Point const &other){
        if(std::abs(this->x - other.x) >= 2 || std::abs(this->y - other.y) >= 2){
            this->x += other.x == this->x ? 0 : other.x > this->x ? 1 : -1;
            this->y += other.y == this->y ? 0 : other.y > this->y ? 1 : -1;;
        }
    }

};

int main(int argc , char** argv) {
    if(argc < 2){
        std::cout << "Missing input file on command line" << std::endl;
    }
    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string line;

    std::set<Point> positions_1;
    std::set<Point> positions_2;

    positions_1.insert(Point(0,0));
    positions_2.insert(Point(0,0));

    std::vector<Point> knots (10, Point(0,0));
    for(auto knot : knots){
        knot.x = 0;
        knot.y = 0;
    }

    while(std::getline(input_file, line)){
        auto count = std::stoi(line.substr(2));
        for (int i=0; i< count; i++){
            switch (line[0]) {
                case 'U':
                    knots[0].y +=1;
                    break;
                case 'D':
                    knots[0].y -=1;
                    break;
                case 'R':
                    knots[0].x +=1;
                    break;
                case 'L':
                    knots[0].x -=1;
                    break;
                default:
                    break;
            }
            for(int i=1; i<10; i++){
                knots[i].follow(knots[i-1]);
            }
            positions_1.insert(knots[1]);
            positions_2.insert(knots[9]);
        }
    }
    std::cout << "Part 1: " << positions_1.size() << std::endl;
    std::cout << "Part 2: " << positions_2.size() << std::endl;
}
