// tribool.cpp
//
#include <iostream>

#include <boost/logic/tribool.hpp>
#include <boost/logic/tribool_io.hpp>

namespace test {

void print_and_or(const boost::logic::tribool& b1, const boost::logic::tribool& b2)
{
    std::cout << std::boolalpha
              << (b1 || b2) << ' ' << (b1 && b2) << '\n';
}

}

int main(int argc, char* argv[])
{
    using namespace boost::logic;

    test::print_and_or(true, indeterminate);
    test::print_and_or(false, indeterminate);
    test::print_and_or(indeterminate, indeterminate);
}
