// asio_steady_timer.cpp
//
#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/phoenix.hpp>
#include <boost/thread.hpp>

BOOST_PHOENIX_ADAPT_FUNCTION(
    void
  , boost_sleep_for
  , boost::this_thread::sleep_for
  , 1
)

int main(int argc, char* argv[])
{
    using namespace boost::asio;

    io_service service;

    boost::chrono::seconds one_second(1);

    using boost::phoenix::val;
    steady_timer t1(service, boost::chrono::seconds(3));
    t1.async_wait(( boost_sleep_for(boost::cref(one_second))
                  , std::cout << val('[') << boost::phoenix::bind(&boost::this_thread::get_id) << "] "
                              << val("after three seconds") << '\n'
                  ));

    steady_timer t2(service, boost::chrono::seconds(3));
    t2.async_wait(std::cout << val('[') << boost::phoenix::bind(&boost::this_thread::get_id) << "] "
                            << val("three seconds passed") << '\n');

    boost::thread th1(boost::phoenix::bind(&io_service::run, boost::ref(service)));
    boost::thread th2(boost::phoenix::bind(&io_service::run, boost::ref(service)));

    th1.join();
    th2.join();
}
