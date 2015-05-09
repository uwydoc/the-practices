// program_options.cpp
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace test {

void on_age(int age)
{
    std::cout << "'age' parsed: " << age << '\n';
}

void to_cout(const std::vector<std::string>& v)
{
    std::copy(v.begin(), v.end(),
            std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout << '\n';
}

}

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    try {
        std::string config_file;

        po::options_description general_opt_desc("General options");
        general_opt_desc.add_options()
            ("help,h", "Output (this) help information")
            ("pi", po::value<float>()->default_value(3.14f), "Custom PI value")
            ("age", po::value<int>()->notifier(test::on_age)->implicit_value(18), "Your age")
            ("phone", po::value<std::vector<std::string> >()->multitoken()->zero_tokens()->composing(), "Phone numbers, multiple values supported, this option can also be specified multiple times")
            ("config,c", po::value<std::string>(&config_file), "Config file path");
        po::positional_options_description general_pos_desc;
        general_pos_desc.add("name", 0);

        po::options_description file_opt_desc("File options");
        file_opt_desc.add_options()
            ("pi", po::value<float>(), "Custom PI value")
            ("age", po::value<int>(), "Your age");

        po::command_line_parser parser(argc, argv);
        parser.options(general_opt_desc)
            .positional(general_pos_desc)
            .allow_unregistered()
            .style(po::command_line_style::default_style
                    | po::command_line_style::allow_slash_for_short);
        po::parsed_options options = parser.run();

        po::variables_map vm;
        po::store(options, vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << general_opt_desc << '\n';
            exit(0);
        } else if (vm.count("config")) {
            std::cout << "config: " << config_file << '\n';
            std::ifstream ifs(config_file.c_str());
            if (ifs)
                po::store(po::parse_config_file(ifs, file_opt_desc), vm);
        }

        if (vm.count("pi"))
            std::cout << "pi: " << vm["pi"].as<float>() << '\n';
        if (vm.count("age"))
            std::cout << "age: " << vm["age"].as<int>() << '\n';
        if (vm.count("phone")) {
            std::cout << "phone: ";
            test::to_cout(vm["phone"].as<std::vector<std::string> >());
        }
        if (vm.count("name"))
            std::cout << "name: " << vm["name"].as<std::string>() << '\n';
    } catch (const po::error& e) {
        std::cerr << e.what() << '\n';
    }
}
