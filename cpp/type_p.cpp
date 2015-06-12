// type_p.cpp
//
// Check whether or not a class/struct has '::type' facility
//
#include <type_traits>
#include <typeinfo>
#include <iostream>

namespace clang {

namespace detail {

    template<class T>
    struct __has_trait_type {
    private:
        struct __one { char _;  };
        struct __two { char _[2]; };

        template<class _Fa> static __one __test(...);
        template<class _Tr> static __two __test(typename _Tr::type* _);
    public:
        static const bool value = sizeof(__test<T>(0)) == sizeof(__two);
    };

    template<class T, bool>
    struct __add_trait_type {
        typedef T type;
    };

    template<class T>
    struct __add_trait_type<T, true> {
        typedef typename T::type type;
    };

}

template<class T>
struct _type: detail::__add_trait_type<T, detail::__has_trait_type<T>::value> {};

}

namespace ownesss {
    
template<class T>
struct _type {
private:
    ///@note declared as 'static' to work around compile error C2352 reported
    ///      by MSVC 10.0
    template<class _Fa> static _Fa __test(...);
    template<class _Tr> static typename _Tr::type __test(typename _Tr::type* _);
public:
    typedef decltype(__test<T>(0)) type;
};

}

namespace test {
    
struct ok { typedef double type; };

struct no {  };

}

int main()
{
    std::cout << typeid(double).name() << std::endl;
    std::cout << "* clang impl" << std::endl;
    std::cout << typeid(clang::_type<test::ok>::type).name() << std::endl;
    std::cout << typeid(clang::_type<test::no>::type).name() << std::endl;
    std::cout << "* ownesss' impl" << std::endl;
    std::cout << typeid(ownesss::_type<test::ok>::type).name() << std::endl;
    std::cout << typeid(ownesss::_type<test::no>::type).name() << std::endl;
}
