// random.cpp
//
#include <stdint.h>
#include <ctime>
#include <iostream>

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

int main(int argc, char* argv[])
{
    namespace random = boost::random;

    std::time_t now = std::time(0);
    random::mt19937 rand(static_cast<uint32_t>(now));
    std::cout << rand() << '\n';

    random::random_device rand_dev;
    random::uniform_int_distribution<> dist(1, 100);
    std::cout << dist(rand_dev) << '\n';
    random::bernoulli_distribution<> dist1;
    std::cout << dist1(rand_dev) << '\n';
}
