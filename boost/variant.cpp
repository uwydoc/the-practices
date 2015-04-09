// variant.cpp
//
#include <iostream>
#include <string>

#include <boost/variant.hpp>

namespace test {

struct output : public boost::static_visitor<>
{
    template<typename T>
    void operator()(const T& t) const
    {
        std::cout << t << '\n';
    }
};

}

int main(int argc, char* argv[])
{
    boost::variant<double, char, std::string> v;
    v = 3.14;
    boost::apply_visitor(test::output(), v);
    v = 'A';
    boost::apply_visitor(test::output(), v);
    v = "Boost";
    boost::apply_visitor(test::output(), v);
}
