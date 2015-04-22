// interprocess_synchronization.cpp
//
#include <iostream>
#include <string>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

int main(int argc, char* argv[])
{
    using namespace boost::interprocess;
    namespace interprocess = boost::interprocess;

    shared_memory_object::remove("managed_shm");
    managed_shared_memory managed_shm(interprocess::open_or_create, "managed_shm", 1024);
    int* int_ptr = managed_shm.find_or_construct<int>("integer")(0);
    /// named interprocess mutex, managed by os
    named_mutex named_mtx(interprocess::open_or_create, "named_mtx");
    /// anonymous(as to the os) interprocess condition variable, stored in shared memory
    //interprocess_condition* cnd_ptr = managed_shm.find_or_construct<interprocess_condition>("cnd")();
    named_condition named_cnd(interprocess::open_or_create, "named_cnd");
    named_condition* cnd_ptr = &named_cnd;

    scoped_lock<named_mutex> lock(named_mtx);
    while (*int_ptr < 10) {
        if (*int_ptr % 2 == 1)
            std::cout << *int_ptr << '\n';
        ++(*int_ptr);
        cnd_ptr->notify_all();
        cnd_ptr->wait(lock);
    }
    cnd_ptr->notify_all();
    shared_memory_object::remove("managed_shm");
    named_mutex::remove("named_mtx");
    ///@note no need to destroy the anonymous condition variable, cause it is
    /// created in shared memory
    named_condition::remove("named_cnd");
}
