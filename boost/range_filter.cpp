// range_filter.cpp
//
#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/phoenix/phoenix.hpp>

int main(int argc, char* argv[])
{
    int a[] = { 5, 2, 3, 1, 0, 4 };
    std::vector<int> v(sizeof(a));
    std::copy(boost::begin(a), boost::end(a), v.begin());

    boost::copy(boost::adaptors::filter(v, boost::phoenix::placeholders::arg1 > 2),
                std::ostream_iterator<int>(std::cout, ", "));
}
