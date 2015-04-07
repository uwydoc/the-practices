// multi_index_key_extractor.cpp
//
#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace test {

class animal
{
public:
    animal(const std::string& name, int legs) : name_(name), legs_(legs) {}

    bool operator<(const animal& other) const { return legs_ < other.legs_; }
    const std::string& name() const { return name_; }
    int legs() const { return legs_; }
private:
    std::string name_;
    int legs_;
};

std::ostream& operator<<(std::ostream& out, const animal& a)
{
    return (out << a.name() << ',' << a.legs());
}

typedef boost::multi_index::multi_index_container<
    animal,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<boost::multi_index::identity<animal> >,
        boost::multi_index::hashed_unique<
            boost::multi_index::const_mem_fun<animal,
                                              const std::string&,
                                              &animal::name> >
    >
> animals_multi;

}

int main(int argc, char* argv[])
{
    test::animals_multi animals;

    animals.emplace("cat", 4);
    animals.emplace("shark", 0);
    animals.emplace("dog", 4);
    animals.emplace("spider", 8);

    std::cout << animals.begin()->name() << '\n';

    typedef test::animals_multi::nth_index<1>::type name_index_type;
    const name_index_type& name_index = animals.get<1>();
    for (name_index_type::iterator it = name_index.begin(); it != name_index.end(); ++it)
        std::cout << *it << '\n';
}
