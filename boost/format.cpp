// format.cpp
//
#include <iostream>
#include <string>

#include <boost/format.hpp>

struct animal
{
    animal(const std::string& name, int legs): name(name), legs(legs) {}
    std::string name;
    int legs;
};

std::ostream& operator<<(std::ostream& os, const animal& a)
{
    return (os << a.name << ',' << a.legs);
}

int main(int argc, char* argv[])
{
    animal cat("cat", 4);
    std::cout << boost::format("%1%") % cat << '\n';

    std::cout << boost::format("%|+| %|| %||") % 1 % 2 % 1 << '\n';

    std::cout << boost::format("%|1$+| %2% %1%") % 1 % 2 << '\n';
}
