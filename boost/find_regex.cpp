// find_regex.cpp
//
#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

int main(int argc, char* argv[])
{
    std::string s = "Boost C++ Libraries";
    boost::iterator_range<std::string::iterator> r =
        boost::algorithm::find_regex(s, boost::regex("\\w\\+\\+"));
    std::cout << r << '\n';
}
