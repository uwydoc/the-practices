// shared_memory.cpp
//
#include <cstdio>
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace test {

void atomic_func(boost::interprocess::managed_shared_memory& managed_shm)
{
    int* ptr1 = managed_shm.construct<int>("integer1")(1);
    float* ptr2 = managed_shm.construct<float>("float1")(2.1);
    std::cout << std::hex << ptr1 << ' ' << ptr2 << '\n';
}

struct atomic_functor
{
    atomic_functor(boost::interprocess::managed_shared_memory& managed_shm) : managed_shm(managed_shm)
    {}

    void operator()() {
        std::cout << __func__ << '\n';
    }

    boost::interprocess::managed_shared_memory& managed_shm;
};

}

int main(int argc, char* argv[])
{
    using namespace boost::interprocess;
    namespace interprocess = boost::interprocess;

    const char* k_shm_name = "Boost";
    const int k_shm_size = 1024;

    shared_memory_object shm(interprocess::open_or_create, k_shm_name, interprocess::read_write);
    shm.truncate(k_shm_size);
    remove_shared_memory_on_destroy shm_guard(k_shm_name);

    mapped_region region(shm, interprocess::read_write);
    char* ptr = static_cast<char*>(region.get_address());
    sprintf(ptr, "hello world");

    mapped_region region1(shm, interprocess::read_only);
    const char* ptr1 = static_cast<char*>(region1.get_address());
    std::cout << std::string(ptr1) << '\n';

    // allocator and container
    shared_memory_object::remove(k_shm_name);
    managed_shared_memory managed_shm(interprocess::open_or_create, k_shm_name, k_shm_size);
    int* ptr2 = managed_shm.construct<int>("integer")(10);
    std::cout << *ptr2 << '\n';
    int* ptr3 = managed_shm.construct<int>("integer_array")[3](11);
    std::pair<int*, std::size_t> p = managed_shm.find<int>("integer_array");
    for (int i = 0; i < p.second; ++i)
        std::cout << p.first[i] << '\n';
    if (managed_shm.destroy<int>("integer_array"))
        std::cout << "integer_array destroyed" << '\n';
    typedef interprocess::allocator<char, managed_shared_memory::segment_manager> char_allocator;
    typedef interprocess::basic_string<char, std::char_traits<char>, char_allocator> interprocess_string;
    interprocess_string* s = managed_shm.find_or_construct<interprocess_string>("interprocess_string")("hello", managed_shm.get_segment_manager());
    s->insert(5, " world");
    std::cout << "interprocess_string: " << *s << '\n';

    // atomic_func
    //managed_shm.atomic_func(boost::bind(test::atomic_func, boost::ref(managed_shm)));
    auto atomic_func = boost::bind(test::atomic_func, boost::ref(managed_shm));
    managed_shm.atomic_func(atomic_func);
}
