// to_upper.cpp
//
#include <iostream>
#include <string>
#include <locale>

#include <boost/algorithm/string.hpp>

int main()
{
    std::string s = "Boost C++ k\xfct\xfcphaneleri";
    std::cout << boost::algorithm::to_upper_copy(s) << '\n';
    std::locale::global(std::locale("tr_TR"));
    std::cout << boost::algorithm::to_upper_copy(s, std::locale("tr_TR")) << '\n';
}
