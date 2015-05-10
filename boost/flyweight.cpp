// flyweight.cpp
//
#include <iostream>
#include <string>
#include <vector>

#include <boost/flyweight.hpp>
#include <boost/flyweight/set_factory.hpp>
#include <boost/flyweight/no_locking.hpp>
#include <boost/flyweight/no_tracking.hpp>

namespace test {

struct person
{
    person(int id, const std::string& city, const std::string& country)
        : id(id), city(city), country(country) {}

    int id;
    std::string city;
    std::string country;
};

struct city_t {};
struct country_t {};

struct person_fw
{
    person_fw(int id, const std::string& city, const std::string& country)
        : id(id), city(city), country(country) {}

    int id;
    boost::flyweights::flyweight<
        std::string
        , boost::flyweights::tag<city_t> > city;
    boost::flyweights::flyweight<
        std::string
        , boost::flyweights::tag<country_t>
        , boost::flyweights::set_factory<>
        , boost::flyweights::no_locking
        , boost::flyweights::no_tracking> country;
};

}

int main(int argc, char* argv[])
{
    do {
        std::vector<test::person> v;
        for (int i = 0; i < 100000; ++i)
            v.push_back(test::person(i, "Hangzhou", "China"));
        getchar();
    } while (0);

    do {
        std::vector<test::person_fw> v;
        for (int i = 0; i < 100000; ++i)
            v.push_back(test::person_fw(i, "Hangzhou", "China"));;
        getchar();
    } while (0);
}
