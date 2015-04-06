// spirit_actions.cpp
//
#include <iostream>
#include <string>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>

int main(int argc, char* argv[])
{
    std::string s;
    std::getline(std::cin, s);
    std::string::iterator it = s.begin();
    int i;
    namespace qi = boost::spirit::qi;
    bool match = qi::phrase_parse(it, s.end(),
            ///@note Requires C++11 support for lambda expression, use
            ///      Boost.Phoenix to workaround
            //qi::int_[([&i](int in) {
            //    std::cout << "[spirit][action] " << in << '\n';
            //    i = in;
            //})],
            qi::int_[boost::phoenix::ref(i) = qi::_1],
            boost::spirit::ascii::space);
    std::cout << std::boolalpha << match << '\n';
    if (match)
        std::cout << i << '\n';
    if (it != s.end())
        std::cout << std::string(it, s.end()) << '\n';
}
