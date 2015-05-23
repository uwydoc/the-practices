// template_as_template_param.cpp
//
// Use explicit template as template parameter, to eliminate duplication
// in template instantiation in some situation
//
#include <iostream>

template<typename T>
class Cache {};

template<typename T>
class Store {};

template<typename T, typename TCache>
class CachedStore
{
    TCache cache;
    Store<T> store;
};

/// explicit template in template params
template<typename T, template <typename> class CacheT>
class CachedStore2
{
    CacheT<T> cache;
    Store<T> store;
};

int main(int argc, char* argv[])
{
    CachedStore<int, Cache<int> > store;
    CachedStore2<int, Cache> store2;
}
