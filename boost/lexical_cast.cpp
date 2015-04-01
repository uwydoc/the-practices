// lexical_cast.cpp
//
#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

struct foo {
    foo(int m = 0): m(m) {}
    int m;
};

std::ostream& operator<<(std::ostream& os, const foo& f)
{
    os << "foo[" << f.m << "]";
    return os;
}

std::istream& operator>>(std::istream& is, foo& f)
{
    char head[4] = { 0 };
    is.read(head, 3);
    assert(std::string(head) == "foo");
    int ch = is.get();
    assert(ch == '[');
    is >> f.m;
    ch = is.get();
    assert(ch == ']');
}

int main(int argc, char* argv[])
{
    foo f(10);
    std::string s = boost::lexical_cast<std::string>(f);
    std::cout << s << '\n';
    foo f1 = boost::lexical_cast<foo>(s);
    std::cout << f1 << '\n';
}
