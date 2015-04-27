// lockfree.cpp
//
#include <iostream>

#include <boost/atomic.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/thread.hpp>

namespace test {

boost::lockfree::queue<int> q(100);
boost::atomic<int> sum(0);

void produce()
{
    for (int i = 0; i < 100; ++i)
        q.push(i);
}

void consume()
{
    using boost::phoenix::placeholders::arg1;
    while (q.consume_one(boost::phoenix::ref(sum) += arg1))
        ;
}

}

int main(int argc, char* argv[])
{
    std::cout << test::sum.is_lock_free() << '\n';

    boost::thread prod_th(test::produce);
    boost::thread cons_th1(test::consume);
    boost::thread cons_th2(test::consume);
    prod_th.join();
    cons_th1.join();
    cons_th2.join();
    using boost::phoenix::placeholders::arg1;
    test::q.consume_all(boost::phoenix::ref(test::sum) += arg1);
    std::cout << test::sum << '\n';
}
