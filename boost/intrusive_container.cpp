// intrusive_container.cpp
//
#include <iostream>
#include <string>

#include <boost/intrusive/list.hpp>
#include <boost/phoenix/phoenix.hpp>

namespace test {

struct animal : boost::intrusive::list_base_hook<>
{
public:
    animal(const std::string& name, int legs) : name(name), legs(legs) {}

    std::string name;
    int legs;
};

std::ostream& operator<<(std::ostream& out, const animal& a)
{
    return (out << a.name << ',' << a.legs);
}

}

int main(int argc, char* argv[])
{
    test::animal a1("cat", 4);
    test::animal a2("shark", 0);
    test::animal* a3 = new test::animal("duck", 2);
    test::animal* a4 = new test::animal("spider", 8);

    typedef boost::intrusive::list<test::animal> animal_list;
    animal_list animals;

    animals.push_back(a1);
    animals.push_back(a2);
    animals.push_back(*a3);
    animals.push_back(*a4);

    // modify
    a2.name = "dolphin";
    a4->legs = 16;

    for (animal_list::iterator it = animals.begin(); it != animals.end(); ++it)
        std::cout << *it << '\n';

    // normal element destroy procedure: destroy after removal
    animals.pop_back();
    delete a4;
    // use `pop_back_and_dispose` interface
    animals.pop_back_and_dispose(
            boost::phoenix::delete_(boost::phoenix::placeholders::arg1));

    std::cout << "number of animals left: " << animals.size() << '\n';
}
