// exception.cpp
//
#include <exception>
#include <iostream>
#include <limits>
#include <new>
#include <string>

#include <boost/exception/all.hpp>

namespace test {

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info;

struct allocation_failed : public std::exception
{
    const char* what() const throw () { return "allocation failed"; }
};

char* allocate_memory(std::size_t size)
{
    char* p = new (std::nothrow) char[size];
    if (!p)
        BOOST_THROW_EXCEPTION(allocation_failed());
    return p;
}

char* write_lots_of_zeros()
{
    try {
        std::size_t max = std::numeric_limits<std::size_t>::max();
        char* p = allocate_memory(max);
        std::fill_n(p, max, 0);
        return p;
    } catch (boost::exception& e) {
        e << errmsg_info("writing lots of zeros failed");
        throw;
    }
}

}

int main(int argc, char* argv[])
{
    try {
        char* p = test::write_lots_of_zeros();
        delete[] p;
    } catch (boost::exception& e) {
        std::cerr << boost::diagnostic_information(e);
        std::cerr << "error info: " << *boost::get_error_info<test::errmsg_info>(e);
    }
}
