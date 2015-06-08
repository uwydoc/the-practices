// test.cpp
//
#include <iostream>
#include <type_traits>

template<typename T, typename std::enable_if<std::is_same<T, int>::value, void>::type* = nullptr>
void match(T t)
{
    std::cout << "match int" << std::endl;
}

template<typename T, typename std::enable_if<std::is_same<T, double>::value, void>::type* = nullptr>
void match(T t)
{
    std::cout << "match double" << std::endl;
}

template<typename T, typename Enabler = void>
struct matcher;

template<typename T>
struct matcher<T, typename std::enable_if<std::is_same<T, int>::value>::type>
{
    matcher(T data) : data(data) {}
    T data;
};
template<typename T>
struct matcher<T, typename std::enable_if<std::is_same<T, double>::value>::type>
{
    matcher(T data) : data(data) {}
    T data;
};

int main()
{
    match(1);
    match(1.1);

    std::cout << matcher<int>(1).data << std::endl;
    std::cout << matcher<double>(1.1).data << std::endl;
}
