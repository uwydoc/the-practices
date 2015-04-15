// asio_http_get_root.cpp
//
#include <iostream>
#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

namespace g {

std::string hostname = "theboostcpplibraries.com";
int port = 80;

boost::asio::io_service ioservice;
boost::asio::ip::tcp::resolver resolver(ioservice);
boost::asio::ip::tcp::socket socket(ioservice);
boost::array<char, 4096> data;

}

namespace test {

void read_handler(const boost::system::error_code err, std::size_t bytes)
{
    if (!err) {
        std::cout << "[info] data read from socket:" << '\n';
        std::cout.write(g::data.data(), bytes);
        std::cout << '\n';
        g::socket.async_read_some(boost::asio::buffer(g::data), read_handler);
    }
}

void connect_handler(const boost::system::error_code err)
{
    if (!err) {
        std::string req = "GET / HTTP/1.1\r\nHost: " + g::hostname + "\r\n\r\n";
        boost::asio::write(g::socket, boost::asio::buffer(req));
        g::socket.async_read_some(boost::asio::buffer(g::data), read_handler);
    }
}

void resolve_handler(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator it)
{
    if (!err) {
        g::socket.async_connect(*it, connect_handler);
    }
}

}

int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 3) {
        std::cout << "Usage: asio_http_get_root <hostname> [port]" << '\n';
        return 0;
    }

    g::hostname = argv[1];
    g::port = (argc == 3) ? boost::lexical_cast<int>(argv[2]) : 80;

    boost::asio::ip::tcp::resolver::query query(g::hostname, boost::lexical_cast<std::string>(g::port));
    g::resolver.async_resolve(query, test::resolve_handler);

    g::ioservice.run();
}
