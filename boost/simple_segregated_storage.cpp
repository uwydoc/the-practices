// simple_segregated_storage.cpp
//
#include <iostream>
#include <vector>
#include <cstddef>

#include <boost/pool/simple_segregated_storage.hpp>

int main(int argc, char* argv[])
{
    boost::simple_segregated_storage<std::size_t> storage;
    std::vector<char> v(1024);
    storage.add_block(&v.front(), v.size(), 256);

    int *i = static_cast<int*>(storage.malloc());
    *i = 1;

    int *j = static_cast<int*>(storage.malloc_n(1, 512));
    j[10] = 2;

    std::cout << *i << ' ' << j[10] << '\n';

    storage.free(i);
    storage.free_n(j, 1, 512);
}
