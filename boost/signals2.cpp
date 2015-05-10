// signals2.cpp
//
#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/phoenix/phoenix.hpp>

namespace test {

struct greeter
{
    void greet() const
    {
        std::cout << "greeting, sir!" << '\n';
    }
};

struct speaker
{
    speaker(const std::string& name) : name(name) {}

    void speak() const
    {
        std::cout << "i'm speaker @" << name << '\n';
    }

    std::string name;
};

void connect_foo(boost::signals2::signal<void()>& s)
{
    s.connect(std::cout << boost::phoenix::val("foo slot"));
}

}

int main(int argc, char* argv[])
{
    namespace signals2 = boost::signals2;
    namespace phoenix = boost::phoenix;
    using boost::phoenix::placeholders::arg1;

    signals2::signal<void()> s;
    s.connect(std::cout << phoenix::val("hello\n"));
    signals2::connection wc = s.connect(std::cout << phoenix::val("world\n"));
    signals2::connection hc = s.connect(1, std::cout << phoenix::val("hey!\n"));
    std::cout << "--- all connected" << '\n';
    s();

    std::cout << "--- disconnect 'world', block 'hey'" << '\n';
    wc.disconnect();
    signals2::shared_connection_block b(hc);
    s();

    std::cout << "--- unblock 'hey'" << '\n';
    b.unblock();
    std::cout << std::boolalpha << "--- hey blocking: " << b.blocking() << '\n';
    s();

    std::cout << "--- connect greeter::greet" << '\n';
    test::greeter greeter;
    s.connect(boost::bind(&test::greeter::greet, &greeter));
    s();

    std::cout << "--- connect speaker::speak with destruction tracking" << '\n';
    boost::shared_ptr<test::speaker> lei_ptr(new test::speaker("lei"));
    s.connect(signals2::signal<void()>::slot_type(&test::speaker::speak, lei_ptr.get()).track(lei_ptr));
    s();

    std::cout << "--- destruct the object with member function associated with the signal" << '\n';
    lei_ptr.reset();
    s();

    std::cout << "--- connect and disconnect during slot" << '\n';
    s.connect(boost::bind(&test::connect_foo, boost::ref(s)));
    s.connect(0, boost::bind(&signals2::connection::disconnect, &hc));
    s();
}
