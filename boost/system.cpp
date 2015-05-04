// system.cpp
//
#include <iostream>
#include <sstream>
#include <string>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace test {

class app_category : public boost::system::error_category
{
public:
    const char* name() const { return "app::system"; }
    std::string message(int ev) const
    {
        std::ostringstream oss;
        oss << this->name() << " error code #" << ev;
        return oss.str();
    }
};

app_category app_cat;

void to_fail(boost::system::error_code& err)
{
    namespace errc = boost::system::errc;
    err = errc::make_error_code(errc::not_supported);
}

void to_fail_and_throw()
{
    namespace errc = boost::system::errc;
    boost::system::error_code err(12, app_cat);
    throw boost::system::system_error(err);
}

}

int main(int argc, char* argv[])
{
    boost::system::error_code err;
    test::to_fail(err);
    std::cerr << err.value() << '\n';
    std::cerr << err.category().name() << '\n';
    try {
        test::to_fail_and_throw();
    } catch (const boost::system::system_error& e) {
        boost::system::error_code err = e.code();
        boost::system::error_condition cond = err.default_error_condition();
        std::cerr << cond.value() << '\n';
        std::cerr << cond.category().name() << '\n';
    }
}
