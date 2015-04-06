// spirit_grammar.cpp
//
#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

typedef boost::variant<int, bool> int_or_bool;

struct int_or_bool_values
{
    int_or_bool first;
    std::vector<int_or_bool> others;
};

BOOST_FUSION_ADAPT_STRUCT(
    int_or_bool_values,
    (int_or_bool, first)
    (std::vector<int_or_bool>, others)
)

namespace test {

template<typename Iterator, typename Skipper>
struct grammar : boost::spirit::qi::grammar<Iterator, int_or_bool_values(), Skipper>
{
    grammar() : grammar::base_type(values)
    {
        value = boost::spirit::qi::int_ | boost::spirit::qi::bool_;
        values = value >> ',' >> value % ',';
    }

    boost::spirit::qi::rule<Iterator, int_or_bool(), Skipper> value;
    boost::spirit::qi::rule<Iterator, int_or_bool_values(), Skipper> values;
};

struct print : boost::static_visitor<>
{
    template<typename T>
    void operator()(const T& t) const
    {
        std::cout << std::boolalpha << t << ';';
    }
};

}

int main(int argc, char* argv[])
{
    std::string s;
    std::getline(std::cin, s);

    namespace qi = boost::spirit::qi;
    std::string::iterator it = s.begin();
    test::grammar<std::string::iterator, boost::spirit::ascii::space_type> g;
    int_or_bool_values v;
    if (qi::phrase_parse(it, s.end(), g, boost::spirit::ascii::space, v)) {
        std::cout << "matched" << '\n';
        test::print p;
        boost::apply_visitor(p, v.first);
        for (int i = 0; i < v.others.size(); ++i) {
            const int_or_bool& elem = v.others[i];
            boost::apply_visitor(p, elem);
        }
    }
}
