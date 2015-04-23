// iostreams_device.cpp
//
#include <iostream>
#include <string>
#include <vector>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

int main(int argc, char* argv[])
{
    using namespace boost::iostreams;
    namespace iostreams = boost::iostreams;

    char buf[] = "hello world\nhow are you?\ni'm fine, thank you. and you?";
    iostreams::array_source buf_source(buf, sizeof(buf) - 1);
    iostreams::stream<iostreams::array_source> buf_in(buf_source);

    std::vector<char> vec;
    typedef iostreams::back_insert_device<std::vector<char> > vector_sink;
    vector_sink vec_sink(vec);
    iostreams::stream<vector_sink> vec_out(vec_sink);
    std::string line;
    std::cout << "[info] read from array source, write to vector sink" << '\n';
    while (std::getline(buf_in, line)) {
        std::cout << "[info][array] " << line << '\n';
        vec_out << line << '\n';
    }
    vec_out << std::flush;

    iostreams::array_source vec_source(vec.data(), vec.size());
    iostreams::stream<iostreams::array_source> vec_in(vec_source);
    iostreams::file_sink f_sink("iostreams_device.txt");
    iostreams::stream<iostreams::file_sink> f_out(f_sink);
    std::cout << "[info] read from vector source, write to file sink" << '\n';
    while (std::getline(vec_in, line)) {
        std::cout << "[info][vector] " << line << '\n';
        f_out << line << '\n';
    }
    f_out << std::flush;
}
