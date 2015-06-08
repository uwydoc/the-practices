// List.h
//
// Persistent `List` implementation normally seen in functional programming
// languages such as Haskell, via C list
//
#ifndef CPP_FUNC_LIST_H_
#define CPP_FUNC_LIST_H_

#include <cassert>

#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

#include <boost/optional.hpp>

template<typename T>
using Option = boost::optional<T>;

struct Void {};

template<typename T>
class List
{
private:
    // C-style linked list
    struct Item
    {
        Item(const T& t, std::shared_ptr<Item> next) : t(t), next(next) {}
        T t;
        const std::shared_ptr<Item> next;
    };

public:
    List() : head_(nullptr) {}
    List(const T& t, const List& tail) : head_(std::make_shared<Item>(t, tail.head_)) {}

    // mimic pattern match
    template<typename R, typename F1, typename F2, typename... Fs>
    R match(F1 f1, F2 f2, Fs... fs)
    {
        bool matched = true;
        try {
            return match<R>(f1);
        } catch (const std::runtime_error& e) {
            matched = false;
            //TODO
        }
        if (!matched) {
            return match<R>(f2, fs...);
        }
    }
    template<typename R>
    R match(std::function<R(void)> f)
    {
        if (!isEmpty())
            throw std::runtime_error("list not empty");
        return f();
    }
    template<typename R>
    R match(std::function<R(T,List<T>)> f)
    {
        if (isEmpty())
            throw std::runtime_error("list empty");
        return f(head(), tail());
    }

    bool isEmpty() const { return !head_; }
    const T& head() const
    {
        assert(!isEmpty());
        return head_->t;
    }
    List tail() const
    {
        assert(!isEmpty());
        return List(head_->next);
    }

private:
    //explicit List(const List& other) : head_(other.head_) {}
    explicit List(std::shared_ptr<Item> item) : head_(item) {}

    const std::shared_ptr<Item> head_;
};

/// makeList
template<typename T>
List<T> makeList()
{
    return List<T>();
}
template<typename T>
List<T> makeList(T t)
{
    return List<T>(t, makeList<int>());
}
template<typename T, typename... Args>
List<T> makeList(T t, Args... args)
{
    return List<T>(t, makeList(args...));
}

/// fmap
template<typename U, typename R = List<U>, typename T, typename F>
R fmap(List<T> lst, F f)
{
    static_assert(std::is_convertible<F, std::function<U(T)> >::value,
            "fmap requires a function of type `U(T)`");
    if (lst.isEmpty())
        return List<U>();
    else
        return List<U>(f(lst.head()), fmap<U>(lst.tail(), f));
}

/// handle 'void' specially, name it 'forEach'
template<typename T, typename F>
void forEach(List<T> lst, F f)
{
    static_assert(std::is_convertible<F, std::function<void(T)> >::value,
            "forEach requires a function of type `void(T)`");
    if (!lst.isEmpty()) {
        f(lst.head());
        forEach(lst.tail(), f);
    }
}

/// filter
template<typename T, typename P>
List<T> filter(List<T> lst, P p)
{
    if (lst.isEmpty())
        return List<T>();
    else if (p(lst.head()))
        return List<T>(lst.head(), filter(lst.tail(), p));
    else
        return filter(lst.tail(), p);
}

/// foldl and foldr
template<typename Acc, typename T, typename F>
Acc foldl(List<T> lst, Acc acc, F f)
{
    static_assert(std::is_convertible<F, std::function<Acc(Acc,T)> >::value,
            "foldl requires a function of type `Acc(Acc,T)`");
    if (lst.isEmpty())
        return acc;
    else
        return foldl(lst.tail(), f(acc, lst.head()), f);
}
template<typename Acc, typename T, typename F>
Acc foldr(List<T> lst, Acc acc, F f)
{
    static_assert(std::is_convertible<F, std::function<Acc(T,Acc)> >::value,
            "foldr requires a function of type `Acc(T,Acc)`");
    if (lst.isEmpty())
        return acc;
    else
        return f(lst.head(), foldr(lst.tail(), acc, f));
}


/// stream out
template<typename T>
std::ostream& operator<<(std::ostream& os, List<T> lst)
{
    os << "List(";
    if (!lst.isEmpty()) {
        os << lst.head();
        forEach(lst.tail(), [&os](T t) { os << ", " << t; });
    }
    os << ')';
}

/// concat
template<typename T>
List<T> concat(List<T> a, List<T> b)
{
    if (a.isEmpty())
        return b;
    return List<T>(a.head(), concat(a.tail(), b));
}

/// reverse, via 'foldl'
template<typename T>
List<T> reverse(List<T> lst)
{
    return foldl(lst, List<T>(), [](List<T> acc, T t) { return List<T>(t, acc); });
}

/// matchOption
template<typename R, typename T, typename F1, typename F2, typename... Fs>
Option<R> matchOption(List<T> lst, F1 f1, F2 f2, Fs... fs)
{
    Option<R> opt = matchOption<R>(lst, f1);
    if (!!opt)
        return opt;
    return matchOption<R>(lst, f2, fs...);
}
template<typename R, typename T, typename F
        , typename std::enable_if<std::is_convertible<F, std::function<R(void)> >::value>::type* = nullptr>
Option<R> matchOption(List<T> lst, F f)
{
    if (lst.isEmpty())
        return Option<R>(f());
    else
        return Option<R>();
}
template<typename R, typename T, typename F
        , typename std::enable_if<std::is_convertible<F, std::function<R(T,List<T>)> >::value>::type* = nullptr>
Option<R> matchOption(List<T> lst, F f)
{
    if (!lst.isEmpty())
        return Option<R>(f(lst.head(), lst.tail()));
    else
        return Option<R>();
}
//template<typename R, typename T>
//Option<R> matchOption(List<T> lst, std::function<R(void)> f)
//{
//    if (lst.isEmpty())
//        return Option<R>(f());
//    else
//        return Option<R>();
//}
//template<typename R, typename T>
//Option<R> matchOption(List<T> lst, std::function<R(T,List<T>)> f)
//{
//    if (!lst.isEmpty())
//        return Option<R>(f(lst.head(), lst.tail()));
//    else
//        return Option<R>();
//}

/// Implement 'match' based on 'matchOption'
template<typename R, typename T, typename F1, typename F2, typename... Fs>
R match(List<T> lst, F1 f1, F2 f2, Fs... fs)
{
    Option<R> opt = matchOption<R>(lst, f1, f2, fs...);
    if (!!opt)
        return opt.get();
    throw std::runtime_error("pattern match failed");
}
template<typename R, typename T, typename F>
R match(List<T> lst, F f)
{
    Option<R> opt = matchOption<R>(lst, f);
    if (!!opt)
        return opt.get();
    throw std::runtime_error("pattern match failed");
}

#endif // CPP_FUNC_LIST_H_
