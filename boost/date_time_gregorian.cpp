// date_time_gregorian.cpp
//
#include <iostream>
#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>

int main(int argc, char* argv[])
{
    namespace greg = boost::gregorian;

    greg::date d(2015, 4, 1);
    std::cout << d.year() << ' '
              << d.month() << ' '
              << d.day() << ' '
              << d.day_of_week() << '\n';
    greg::date d1 = d + greg::date_duration(30);
    greg::date_period dp(d, d1);
    std::cout << std::boolalpha << dp.contains(greg::from_simple_string("2015-04-25")) << '\n';

    greg::date today = greg::day_clock::local_day();
    greg::date two_days_later = today + greg::days(2);
    greg::day_iterator it(today);
    while (*it != two_days_later)
        std::cout << *(++it) << '\n';
}
