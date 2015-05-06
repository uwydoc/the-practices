// accumulators.cpp
//
#include <stdint.h>
#include <iostream>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

int main(int argc, char* argv[])
{
    namespace acc = boost::accumulators;
    acc::accumulator_set<
        int32_t,
        acc::features<acc::tag::count, acc::tag::mean, acc::tag::variance>,
        int
    > as;
    as(8, acc::weight = 1);
    as(9, acc::weight = 1);
    as(10, acc::weight = 4);
    as(11, acc::weight = 1);
    as(12, acc::weight = 1);
    std::cout << acc::count(as) << '\n'
              << acc::mean(as) << '\n'
              << acc::variance(as) << '\n';
}
