// scoped_ptr.cpp
//
// The difference between `boost::scoped_ptr` and `std::unique_ptr` is:
// Instance of `boost::scoped_ptr` cannot be copied or moved, so the ownership
// of the resource/memory it possesses cannot be transfered beyond the scope it
// is defined. So when you see such definition, you know the memory it managed
// would be released for sure outside the scope. However, instance of
// `std::unique_ptr` can be copied, so the ownership can be transfered.
//
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <iostream>

namespace test {

template<typename T>
struct element {
public:
    element() {}
    element(const T& t): val(t) {}
    T val;
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const element<T>& elem)
{
    stream << elem.val;
    return stream;
}

}

int main(int argc, char* argv[])
{
    boost::scoped_ptr<test::element<int> > elem(new test::element<int>(10));
    std::cout << *elem << '\n';
    elem.reset(new test::element<int>(20));
    std::cout << *elem << '\n';
    boost::scoped_array<test::element<int> > elems(new test::element<int>[3]);
    elems[0] = test::element<int>(1);
    elems[1] = test::element<int>(2);
    elems[2] = test::element<int>(3);
    std::cout << elems[0] << ' ' << elems[1] << ' ' << elems[2] << '\n';
}
