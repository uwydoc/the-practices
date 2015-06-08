// ListTest.cpp
//
#include "List.h"

#include <iostream>

template<typename T>
void printList(List<T> lst)
{
    //forEach(lst, [](T x) { std::cout << x << ' '; });
    //std::cout << std::endl;
    std::cout << lst << std::endl;
}

int main()
{
    //List<int> lst(1, List<int>(2, List<int>(3, List<int>())));
    List<int> lst = makeList(1, 2, 3);
    printList(lst);
    std::cout << "--- fmap --- " << '\n';
    List<int> lst1 = fmap<int>(lst, [](int x) { return x + 1; });
    printList(lst1);
    std::cout << "--- filter ---" << '\n';
    List<int> lst2 = filter(lst, [](int x) { return x % 2 == 0; });
    printList(lst2);
    std::cout << "--- foldr ---" << '\n';
    List<double> lst3 = foldr(lst, List<double>(),
            [](int x, List<double> acc) {
                return List<double>(1.0 * x / 3, acc);
            });
    printList(lst3);
    std::cout << "--- foldl ---" << '\n';
    int sum = foldl(lst, 0, [](int acc, int x) { return x + acc; });
    std::cout << sum << std::endl;
    std::cout << "--- match(member) ---" << '\n';
    makeList<int>().match<void>([](void) { std::cout << "empty" << '\n'; });
    makeList<int>(1,2,3).match<Void>(
            []() { std::cout << "empty" << '\n'; return Void(); }
            , [](int head, List<int> tail) {
                std::cout << "head " << head << '\n';
                return Void();
            });
    std::cout << "--- match ---" << '\n';
    matchOption<Void>(makeList<int>()
            , []() { std::cout << "empty" << '\n'; return Void(); }
            , [](int head, List<int> tail) {
                std::cout << "head " << head << '\n';
                return Void();
            });
    matchOption<Void>(makeList<int>(3,2,1)
            , []() { std::cout << "empty" << '\n'; return Void(); }
            , [](int head, List<int> tail) {
                std::cout << "head " << head << '\n';
                return Void();
            });
    try {
        match<Void>(makeList<int>()
                //, []() { std::cout << "empty" << '\n'; return Void(); }
                , [](int head, List<int> tail) {
                    std::cout << "head " << head << '\n';
                    return Void();
                });
    } catch (const std::runtime_error& e) {
        std::cerr << "[error] " << e.what() << std::endl;
    }
}
