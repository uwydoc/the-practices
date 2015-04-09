// unpack_using_tie.cpp
//
#include <iostream>
#include <string>

#include <boost/tuple/tuple.hpp>

namespace test {

boost::tuple<std::string, int, bool> make_cat()
{
    return boost::make_tuple("cat", 4, true);
}

}

int main(int argc, char* argv[])
{
    std::string name;
    int legs;
    bool with_tail;

    boost::tie(name, legs, with_tail) = test::make_cat();

    std::cout << std::boolalpha << name << ',' << legs << ',' << with_tail << '\n';
}
