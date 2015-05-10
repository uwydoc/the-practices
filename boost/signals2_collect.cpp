// signals2_collect.cpp
//
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include <boost/phoenix/phoenix.hpp>
#include <boost/signals2.hpp>

namespace test {

template<typename T>
struct min_element
{
    typedef T result_type;

    template<typename InputIt>
    T operator()(InputIt first, InputIt last) const
    {
        std::vector<T> v(first, last);
        return *std::min_element(v.begin(), v.end());
    }
};

template<typename T>
struct collect
{
    typedef std::vector<T> result_type;

    template<typename InputIt>
    std::vector<T> operator()(InputIt first, InputIt last) const
    {
        return std::vector<T>(first, last);
    }
};

}

int main(int argc, char* argv[])
{
    namespace signals2 = boost::signals2;
    namespace phoenix = boost::phoenix;

    signals2::signal<int()> s;
    s.connect(phoenix::val(1));
    s.connect(phoenix::val(3));
    s.connect(phoenix::val(5));
    std::cout << "default(last result): " << *s() << '\n';

    signals2::signal<int(), test::min_element<int> > s1;
    s1.connect(phoenix::val(1));
    s1.connect(phoenix::val(3));
    s1.connect(phoenix::val(5));
    std::cout << "min element: " << s1() << '\n';

    signals2::signal<int(), test::collect<int> > s2;
    s2.connect(phoenix::val(1));
    s2.connect(phoenix::val(3));
    s2.connect(phoenix::val(5));
    std::vector<int> v = s2();
    std::cout << "collect: ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << '\n';
}
