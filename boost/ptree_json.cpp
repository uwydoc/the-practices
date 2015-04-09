// ptree_json.cpp
//
#include <iostream>
#include <string>

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

int main(int argc, char* argv[])
{
    using boost::property_tree::ptree;

    ptree pt;
    pt.put(ptree::path_type("C:\\Windows\\System32", '\\'), "2436 files");
    pt.put(ptree::path_type("C:\\Program Files\\Notepad2", '\\'), "8 files");

    ptree& program_files = pt.get_child(ptree::path_type("C:\\Program Files", '\\'));
    program_files.put_child("Oracle", ptree("VirtualBox"));

    boost::optional<ptree&> virtualbox = program_files.get_child_optional("Oracle.VirtualBox");
    if (virtualbox)
        virtualbox.get().put("x86", "4 files");

    boost::optional<ptree&> windows = pt.get_child_optional("C:.Windows");
    if (windows)
        windows.get().put("SysWOW64", "2436 files");

    namespace json_parser = boost::property_tree::json_parser;
    json_parser::write_json("ptree.json", pt);

    ptree pt2;
    json_parser::read_json("ptree.json", pt2);

    std::cout << std::boolalpha << (pt2 == pt) << '\n';
}
