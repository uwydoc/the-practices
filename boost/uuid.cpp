// uuid.cpp
//
#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace test {

void print_uuid(const boost::uuids::uuid id)
{
    std::cout << "uuid: " << id << '\n'
              << " size: " << id.size() << '\n'
              << " is_nil: " << std::boolalpha << id.is_nil() << '\n'
              << " variant: " << id.variant() << '\n'
              << " version: " << id.version() << '\n';
}

}

int main(int argc, char* argv[])
{
    namespace uuids = boost::uuids;

    uuids::random_generator rand_gen;
    uuids::uuid rand_id = rand_gen();
    test::print_uuid(rand_id);

    uuids::nil_generator nil_gen;
    uuids::uuid nil_id = nil_gen();
    test::print_uuid(nil_id);

    uuids::string_generator str_gen;
    uuids::uuid str_id = str_gen("CF77C981-F61B-7817-10FF-D916FCC3EAA4");
    test::print_uuid(str_id);
    std::cout << uuids::to_string(str_id) << '\n';

    uuids::name_generator name_gen(str_id);
    uuids::uuid name_id = name_gen("theboostcpplibraries.com");
    test::print_uuid(name_id);
    std::cout << boost::lexical_cast<std::string>(name_id) << '\n';
}
