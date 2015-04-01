// object_pool.cpp
//
#include <iostream>

#include <boost/pool/object_pool.hpp>

int main(int argc, char* argv[])
{
    boost::object_pool<int> pool;

    int *i = pool.malloc();
    *i = 1;
    int *j = pool.construct(2);

    std::cout << *i << ' ' << *j << '\n';
    pool.destroy(i);
    pool.destroy(j);
}
