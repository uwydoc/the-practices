// filesystem_create_and_list.cpp
//
#include <iostream>
#include <fstream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

int main(int argc, char* argv[])
{
    namespace fs = boost::filesystem;

    /// boost::filesystem::path and related operations
    fs::path p = fs::current_path();
    p /= "tmp";
    if (fs::exists(p) && !fs::is_directory(p)) {
        std::cerr << "[error] path \"" << p << "\" exists and is not a directory" << '\n';
        exit(1);
    }
    if (!fs::exists(p))
        fs::create_directory(p);

    /// using boost::filesystem::ifstream/ofstream with boost::filesystem::path
    fs::path hello_path = p / "hello.txt";
    fs::ofstream(hello_path)
        << 1  ///@note [wierd] must append some primitives first before the
              ///      following string literal could be correctly outputed,
              ///      otherwise, some address-like value would be outputed
        << "hello"
        << std::flush;
    fs::path world_path = p / "world.txt";
    std::ofstream(world_path.generic_string().c_str()) << 2 << "world" << std::flush;

    /// list directory entries with `directory_iterator`,
    /// or using `recursive_directory_iterator` if recursive is needed
    fs::directory_iterator it(p);
    while (it != fs::directory_iterator()) {
        const fs::path& tp = it->path();
        std::cout << std::boolalpha << tp << ' ' << fs::is_regular_file(tp) << '\n';
        ++it;
    }
}
