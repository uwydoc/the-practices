// shared_ptr.cpp
//
// Since Boost 1.53.0, `boost::shared_ptr` also support array pointers.
//
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/chrono/system_clocks.hpp>

int main(int argc, char* argv[])
{
    using namespace boost::chrono;
    boost::shared_ptr<int> p;
    steady_clock::time_point then = steady_clock::now();
    for (int i = 0; i < 5000000; ++i)
        p.reset(new int(i));
    steady_clock::time_point now = steady_clock::now();
    boost::chrono::duration<double> elapsed = now - then;
    std::cout << elapsed.count()*1000 << "ms" << '\n';
}
