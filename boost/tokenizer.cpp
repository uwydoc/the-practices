// tokenizer.cpp
//
#include <iostream>
#include <string>

#include <boost/tokenizer.hpp>

int main(int argc, char* argv[])
{
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    std::string s = "Boost C++ Libraries";
    boost::char_separator<char> sep(" ", "+", boost::keep_empty_tokens);
    tokenizer tok(s, sep);
    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
        std::cout << *it << '\n';
}
