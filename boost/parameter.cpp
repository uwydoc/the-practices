// parameter.cpp
//
#include <iostream>
#include <ios>
#include <string>

#include <boost/parameter.hpp>

BOOST_PARAMETER_NAME(a)
BOOST_PARAMETER_NAME(b)
BOOST_PARAMETER_NAME(c)
BOOST_PARAMETER_NAME(d)
BOOST_PARAMETER_NAME(e)

BOOST_PARAMETER_FUNCTION(
    (void),
    complicated,
    tag,
    (required
        (a, (int))
        (b, (char))
        (c, (double))
        (d, (std::string))
        (e, *)
    )
)
{
    std::cout << std::ios::boolalpha
              << a << '\n'
              << b << '\n'
              << c << '\n'
              << d << '\n'
              << e << '\n';
}

int main(int argc, char* argv[])
{
    complicated(_c = 3.14, _a = 1, _d = "Boost.Parameter", _b = 'B', _e = true);
}
