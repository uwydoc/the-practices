// weak_ptr.cpp
//
#include <iostream>
#include <functional>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/weak_ptr.hpp>

namespace test {

void reset(boost::shared_ptr<int>& sp)
{
    sp.reset();
}

void print(boost::weak_ptr<int>& wp)
{
    boost::shared_ptr<int> sp = wp.lock();
    if (sp)
        std::cout << *sp << '\n';
}

}

int main(int argc, char* argv[])
{
    boost::shared_ptr<int> sp(new int(10));
    boost::weak_ptr<int> wp(sp);
    boost::thread t1(test::print, boost::ref(wp));
    boost::thread t2(test::reset, boost::ref(sp));
    t1.join();
    t2.join();
}
