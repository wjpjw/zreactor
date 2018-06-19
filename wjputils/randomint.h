#pragma once

#include <random>

namespace wjp
{
class randint{
public:
    randint(int min, int max):gen(rd()),dis(min, max){}
    int next(){return dis(gen);}
private:
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen;       //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis;
};

}
