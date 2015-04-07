// multi_index.cpp
//
#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/phoenix/phoenix.hpp>

namespace test {

struct animal
{
    std::string name;
    int legs;
    std::string speak;
};

std::ostream& operator<<(std::ostream& out, const animal& a)
{
    return (out << a.name << ',' << a.legs << ',' << a.speak);
}

typedef boost::multi_index::multi_index_container<
    animal,
    boost::multi_index::indexed_by<
        boost::multi_index::sequenced<>,
        boost::multi_index::ordered_unique<
            boost::multi_index::member<animal, std::string, &animal::name> >,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::member<animal, int, &animal::legs> >,
        boost::multi_index::hashed_non_unique<
            boost::multi_index::member<animal, std::string, &animal::speak> >,
        boost::multi_index::random_access<> >
> animals_multi;

}

int main(int argc, char* argv[])
{
    test::animals_multi animals;

    animals.push_back({"cat", 4, "miao"});
    animals.push_back({"dog", 4, "bark"});
    animals.push_back({"spider", 8, "..."});
    animals.push_back({"ape", 2, "woo"});
    animals.push_back({"dolphin", 0, "ming"});

    std::cout << "-- [default] --" << '\n';
    std::cout << *(animals.begin()) << '\n';

    std::cout << "-- sequenced --" << '\n';
    typedef test::animals_multi::nth_index<0>::type seq_index_type;
    const seq_index_type& seq_index = animals.get<0>();
    for (seq_index_type::iterator it = seq_index.begin(); it != seq_index.end(); ++it)
        std::cout << *it << '\n';

    std::cout << "-- ordered_unique --" << '\n';
    typedef test::animals_multi::nth_index<1>::type name_index_type;
    name_index_type& name_index = animals.get<1>();
    for (name_index_type::iterator it = name_index.begin(); it != name_index.end(); ++it)
        std::cout << *it << '\n';
    do {
        name_index_type::iterator it = name_index.find("dolphin");
        if (it != name_index.end()) {
            std::cout << "\"dolphin\" found: " << *it << '\n';
            name_index.modify(it,
                    boost::phoenix::bind(&test::animal::speak,
                                         boost::phoenix::placeholders::arg1)
                    = "bing");
        }
    } while (0);

    std::cout << "-- ordered_non_unique --" << '\n';
    typedef test::animals_multi::nth_index<2>::type legs_index_type;
    const legs_index_type& legs_index = animals.get<2>();
    for (legs_index_type::iterator it = legs_index.begin(); it != legs_index.end(); ++it)
        std::cout << *it << '\n';
    std::cout << "number of animals with 4 legs: " << legs_index.count(4) << '\n';
    do {
        legs_index_type::iterator it = legs_index.lower_bound(4);
        legs_index_type::iterator end = legs_index.upper_bound(8);
        std::cout << "animals with legs number between 4 and 8:" << '\n';
        while (it != legs_index.end() && it != end) {
            std::cout << " - " << *it << '\n';
            ++it;
        }
    } while (0);

    std::cout << "-- hashed_unique --" << '\n';
    typedef test::animals_multi::nth_index<3>::type speak_index_type;
    const speak_index_type& speak_index = animals.get<3>();
    for (speak_index_type::iterator it = speak_index.begin(); it != speak_index.end(); ++it)
        std::cout << *it << '\n';
    do {
        speak_index_type::iterator it = speak_index.find("bark");
        if (it != speak_index.end())
            std::cout << "animal that \"bark\"s: " << *it << '\n';
    } while (0);

    std::cout << "-- random_access --" << '\n';
    typedef test::animals_multi::nth_index<4>::type random_index_type;
    const random_index_type& random_index = animals.get<4>();
    for (int i = 0; i < random_index.size(); ++i)
        std::cout << random_index[i] << '\n';
    std::cout << "the 2nd animal: " << random_index.at(1) << '\n';
}
