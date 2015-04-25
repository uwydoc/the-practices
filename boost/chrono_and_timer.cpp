// chrono_and_timer.cpp
//
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#define BOOST_CHRONO_VERSION 2
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>
#include <boost/timer/timer.hpp>

namespace test {

void consume_some_time()
{
    const static double PI = std::asin(1) * 2;
    for (int i = 0; i < 100000; ++i)
        std::pow(PI, i);
}

}

int main(int argc, char* argv[])
{
    namespace chrono = boost::chrono;
    namespace timer = boost::timer;

    /// use `high_resolution_clock`, it would choose between `system_clock` and
    /// `steady_clock`(if available), whichever has a higher resolution
    chrono::high_resolution_clock::time_point tp = chrono::high_resolution_clock::now();
    std::cout << "current time: " << tp << '\n';
    chrono::minutes m(15);
    tp = tp + m;
    std::cout << chrono::symbol_format
              << m << " later: "
              << chrono::time_fmt(chrono::timezone::local, "%H:%M:%S")
              << chrono::time_point_cast<chrono::seconds>(tp) << '\n';

    timer::cpu_timer t;
    test::consume_some_time();
    std::cout << t.format() << '\n';

    std::ostringstream oss;
    do {
        timer::auto_cpu_timer t(oss);
        test::consume_some_time();
    } while (false);
    std::cout << oss.str() << '\n';
}
