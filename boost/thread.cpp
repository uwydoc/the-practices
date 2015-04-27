// thread.cpp
//
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/ref.hpp>
#include <boost/move/move.hpp>

namespace test {

boost::mutex mtx;
boost::condition_variable_any cond;
std::vector<int> v;

std::string get_thread_id()
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << std::hex << boost::this_thread::get_id();
    return oss.str();
}

class prefixed_ostream
{
public:
    prefixed_ostream(std::ostream& os, const std::string& prefix = std::string("")) :
        os_(os), prefix_(prefix)
    {}

    template<typename T>
    std::ostream& operator<< (const T& t) {
        return (os_ << prefix_ << t);
    }

    std::ostream& ostream() const {
        return os_;
    }

    const std::string& prefix() const {
        return prefix_;
    }

    void set_prefix(const std::string& prefix) {
        prefix_ = prefix;
    }

private:
    std::ostream& os_;
    std::string prefix_;
};

void fill()
{
    prefixed_ostream pout(std::cout, "[" + get_thread_id() + "] ");

    std::srand(static_cast<unsigned int>(std::time(0)));
    try {
        for (int i = 0; i < 5; ++i) {
            boost::unique_lock<boost::mutex> lock(mtx);
            int n = std::rand() % 10;
            v.push_back(n);
            pout << "fill " << n << '\n';
            cond.notify_all();
            cond.wait(mtx);
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }
    } catch (const boost::thread_interrupted& e) {
        boost::lock_guard<boost::mutex> guard(mtx);
        pout << "interrupted" << '\n';
    }
}

void print()
{
    prefixed_ostream pout(std::cout, "[" + get_thread_id() + "] ");

    try {
        for (int i = 0; i < 5; ++i) {
            boost::unique_lock<boost::mutex> lock(mtx);
            while (v.size() == 0)
                cond.wait(mtx);
            int n = v.back();
            pout << "print " << n << '\n';
            v.pop_back();
            cond.notify_all();
        }
    } catch (const boost::thread_interrupted& e) {
        boost::lock_guard<boost::mutex> guard(mtx);
        pout << "interrupted" << '\n';
    }
}

int sum()
{
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += i;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
    }
    return sum;
}

void sum_p(boost::promise<int>& p)
{
    int n = sum();
    p.set_value(n);
}

}

int main(int argc, char* argv[])
{
    /// thread creation and synchronization with mutex and condition variable
    boost::thread fill_th(test::fill);
    boost::thread print_th(test::print);
    boost::this_thread::sleep_for(boost::chrono::seconds(3));
    fill_th.interrupt();
    print_th.interrupt();
    fill_th.join();
    print_th.join();
    /// future and promise
    boost::promise<int> p;
    boost::future<int> f = p.get_future();
    boost::thread sum_p_th(test::sum_p, boost::ref(p));
    std::cout << f.get() << '\n';
    sum_p_th.join();
    /// packaged_task
    boost::packaged_task<int> t(test::sum);
    boost::future<int> f1 = t.get_future();
    boost::thread sum_th(boost::move(t));
    std::cout << f1.get() << '\n';
    sum_th.join();
    /// async, failed compiling, tbi
    //boost::future<int> f2 = boost::async(boost::launch::async, test::sum);
    //std::cout << f2.get() << '\n';
}
