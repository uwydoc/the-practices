// xpressive.cpp
//
#include <iostream>
#include <iterator>
#include <string>

#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>

int main(int argc, char* argv[])
{
    namespace xp = boost::xpressive;
    std::string s = "The Boost Libraries";
    std::ostream_iterator<std::string> it(std::cout, "\n");
    xp::sregex expr = +xp::_w >> xp::_s >> (+xp::_w)[*xp::ref(it) = xp::_] >> xp::_s >> +xp::_w;
    std::cout << std::boolalpha << xp::regex_match(s, expr) << '\n';
}
