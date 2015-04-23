// iostreams_filter.cpp
//
#include <iostream>
#include <string>
#include <vector>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>

int main(int argc, char* argv[])
{
    namespace iostreams = boost::iostreams;

    std::vector<char> v;
    typedef iostreams::back_insert_device<std::vector<char> > vector_sink;
    vector_sink v_sink(v);
    iostreams::filtering_ostream filter_out;
    filter_out.push(iostreams::zlib_compressor());
    filter_out.push(v_sink);
    filter_out << "boost.iostreams\nfilters\nzlib" << std::flush;
    filter_out.pop();

    iostreams::array_source v_source(v.data(), v.size());
    iostreams::filtering_istream filter_in;
    filter_in.push(iostreams::zlib_decompressor());
    filter_in.push(v_source);
    std::string line;
    std::cout << "[info] data decompressed from the vector:" <<  '\n';
    while (std::getline(filter_in, line))
        std::cout << "[info]... " << line << '\n';
}
