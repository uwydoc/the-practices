// escaped_list_separator.cpp
//
#include <iostream>
#include <string>

#include <boost/tokenizer.hpp>

int main(int argc, char* argv[])
{
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    std::string s = "Boost,\"C++ Libraries\"";
    tokenizer tok(s);
    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
        std::cout << *it << '\n';
}
