// regex.cpp
//
#include <iostream>
#include <string>

#define BOOST_REGEX_USE_CPP_LOCALE
#include <boost/regex.hpp>

int main(int argc, char* argv[])
{
    std::string s = "Boost Libraries";
    boost::regex expr("(\\w+)\\s(\\w+)");
    boost::smatch what;
    if (boost::regex_search(s, what, expr)) {
        std::cout << what[0] << '\n';
        std::cout << what[1] << ' ' << what[2] << '\n';
    }
}
