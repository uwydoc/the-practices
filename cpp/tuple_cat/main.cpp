// main.cpp
//
// |tuple_cat| implementation by <Eric Niebler>
// http://ericniebler.com/2014/11/13/tiny-metaprogramming-library/
//

#include <string>
#include <tuple>

// |typelist|
// a class template that is just a list of types
template<typename... T> struct typelist {};

// |as_typelist|
// convert |std::tuple| to |typelist|
template<typename T> struct as_typelist;

template<typename... Ts>
struct as_typelist<std::tuple<Ts...> >
{
    using type = typelist<Ts...>;
};

// |as_typelist_t|
template<typename T>
using as_typelist_t = typename as_typelist<T>::type;

// |typelist_cat|
// concatenate several typelists into one typelist
template<typename... T> struct typelist_cat;

template<typename... Ts>
struct typelist_cat<typelist<Ts...> >
{
    using type = typelist<Ts...>;
};

template<typename... Ts1, typename... Ts2, typename... Rem>
struct typelist_cat<typelist<Ts1...>, typelist<Ts2...>, Rem...>
{
    using type = typename typelist_cat<typelist<Ts1..., Ts2...>, Rem...>::type;
};

// |typelist_cat_t|
template<typename... T>
using typelist_cat_t = typename typelist_cat<T...>::type;

// |typelist_apply|
// expand a typelist into a variadic template, for example, |std::tuple|
template<template<typename...> class C, typename List>
struct typelist_apply;

template<template<typename...> class C, typename... Ts>
struct typelist_apply<C, typelist<Ts...> >
{
    using type = C<Ts...>;
};

// |typelist_apply_t|
template<template<typename...> class C, typename List>
using typelist_apply_t = typename typelist_apply<C, List>::type;

/// Metafunction class, class with a public templated |apply| member
// |meta_apply| apply a metafunction class
template<typename F, typename... Ts>
using meta_apply = typename F::template apply<Ts...>;

// |meta_compose| compose two metafunction class
template<typename... Fs>
struct meta_compose;

template<>
struct meta_compose<>
{
    template<typename T>
    using apply = T;
};

template<typename F>
struct meta_compose<F>
{
    template<typename T>
    using apply = meta_apply<F, T>;
};

template<typename F0, typename... Fs>
struct meta_compose<F0, Fs...>
{
    template<typename T>
    using apply = meta_apply<F0, meta_compose<Fs...> >;
};

// |meta_quote| turns a metafunction into a metafunction class
template<template<typename...> class F>
struct meta_quote
{
    template<typename... Ts>
    using apply = F<Ts...>;
};

/// |tuple_cat| implementation
template<typename... Tuples>
using __tuple_cat_ret_t = typelist_apply_t<
        std::tuple,
        typelist_cat_t<typelist<as_typelist_t<Tuples>...> > >;

template<typename... Tuples>
struct __tuple_cat_fn
{
    auto operator()(Tuples&&... tpls) -> __tuple_cat_ret_t<Tuples...>;
};

template<typename... Ts, typename... Rem>
struct __tuple_cat_fn<std::tuple<Ts...>, Rem...>
{
    auto operator()(std::tuple<Ts...>&& t1, Rem&&... rem) -> __tuple_cat_ret_t<std::tuple<Ts...>, Rem...>
    {
        //
    }
};

template<typename... Tuples>
auto tuple_cat(Tuples&&... tpls) -> __tuple_cat_ret_t<Tuples...>
{
    return __tuple_cat_fn<Tuples...>()(std::move(tpls)...);
}

// main
int main()
{
    typedef std::tuple<int, char, std::string> tuple1_t;
    as_typelist_t<tuple1_t> t1;
    typedef std::tuple<float, double> tuple2_t;
    as_typelist_t<tuple2_t> t2;
    typelist_cat_t<as_typelist_t<tuple1_t>, as_typelist_t<tuple2_t> > t3;

    ::tuple_cat(tuple1_t{1,'a',"hello"});
}
