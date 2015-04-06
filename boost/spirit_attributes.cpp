// spirit_attributes.cpp
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>

int main(int argc, char* argv[])
{
    namespace qi = boost::spirit::qi;
    std::string s;
    std::getline(std::cin, s);
    std::string::iterator it = s.begin();
    std::vector<int> v;
    if (qi::phrase_parse(it, s.end(), qi::int_ % ',', boost::spirit::ascii::space, v)) {
        std::ostream_iterator<int> out(std::cout, ";");
        std::copy(v.begin(), v.end(), out);
    }
}
