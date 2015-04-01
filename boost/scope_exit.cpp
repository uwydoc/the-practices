// scope_exit.cpp
//
#include <iostream>

#include <boost/scope_exit.hpp>

namespace test {

struct foo
{
    int i;

    void bar()
    {
        i = 10;
        BOOST_SCOPE_EXIT(void) {
            std::cout << "first" << std::endl;
        } BOOST_SCOPE_EXIT_END
        BOOST_SCOPE_EXIT(&i) {
            std::cout << "second" << std::endl;
            i = 20;
        } BOOST_SCOPE_EXIT_END
        BOOST_SCOPE_EXIT(this_) {
            std::cout << "third" << std::endl;
            this_->i = 30;
        } BOOST_SCOPE_EXIT_END
    }
};

}

int main(int argc, char* argv[])
{
    test::foo obj;
    obj.bar();
    std::cout << obj.i << std::endl;
}
