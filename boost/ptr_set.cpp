// ptr_set.cpp
//
#include <iostream>
#include <memory>
#include <functional>
#include <set>

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/indirect_fun.hpp>

int main(int argc, char* argv[])
{
    boost::ptr_set<int> s;
    s.insert(new int(2));
    s.insert(new int(1));
    std::cout << *s.begin() << '\n';

    std::set<std::unique_ptr<int>, boost::indirect_fun<std::less<int> > > v;
    v.insert(std::unique_ptr<int>(new int(2)));
    v.insert(std::unique_ptr<int>(new int(1)));
    std::cout << **v.begin() << '\n';
}