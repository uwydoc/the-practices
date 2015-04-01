// ptr_vector.cpp
//
#include <iostream>

#include <boost/ptr_container/ptr_vector.hpp>

int main(int argc, char* argv[])
{
    boost::ptr_vector<int> v;
    v.push_back(new int(1));
    v.push_back(new int(2));
    std::cout << v.back() << '\n';
}
