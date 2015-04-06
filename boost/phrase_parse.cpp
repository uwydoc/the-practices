// phrase_parse.cpp
//
#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>

int main(int argc, char* argv[])
{
    using namespace boost::spirit;
    std::string s;
    std::getline(std::cin, s);
    std::string::iterator it = s.begin();
    bool match = qi::phrase_parse(it, s.end(),
            qi::lexeme[ascii::digit >> ascii::digit]
            >> qi::lexeme[ascii::digit >> +ascii::digit],
            ascii::space,
            //qi::skip_flag::dont_postskip);
            qi::skip_flag::postskip);
    std::cout << std::boolalpha << match << '\n';
    if (it != s.end())
        std::cout << std::string(it, s.end()) << '\n';
}
