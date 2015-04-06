// spirit_rules.cpp
//
#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>

namespace test {

struct print : public boost::static_visitor<>
{
    template<typename T>
    void operator()(const T& t) const
    {
        std::cout << std::boolalpha << t << "; ";
    }
};

}

int main(int argc, char* argv[])
{
    std::string s;
    std::getline(std::cin, s);
    std::string::iterator it = s.begin();

    namespace qi = boost::spirit::qi;
    qi::rule<std::string::iterator, boost::variant<int, bool>(),
        boost::spirit::ascii::space_type> int_or_bool = qi::int_ | qi::bool_ ;
    qi::rule<std::string::iterator, std::vector<boost::variant<int, bool> >(),
        boost::spirit::ascii::space_type> int_or_bools = int_or_bool % ',';
    std::vector<boost::variant<int, bool> > v;
    if (qi::phrase_parse(it, s.end(), int_or_bools,
                boost::spirit::ascii::space, v)) {
        std::cout << "matched" << '\n';
        for (int i = 0; i < v.size(); ++i) {
            const boost::variant<int, bool>& elem = v[i];
            boost::apply_visitor(test::print(), elem);
        }
    }
}
