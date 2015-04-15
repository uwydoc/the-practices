// asio_time_server.cpp
//
#include <iostream>
#include <string>
#include <ctime>

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace g {

int port = 2345;

boost::asio::io_service ioservice;
boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
boost::asio::ip::tcp::acceptor acceptor(ioservice, endpoint);
boost::asio::ip::tcp::socket socket(ioservice);
std::string data;

}

namespace test {

void write_handler(const boost::system::error_code& err, std::size_t bytes)
{
    if (!err) {
        g::socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
    }
}

void accept_handler(const boost::system::error_code& err)
{
    if (!err) {
        std::time_t now = std::time(0);
        g::data = std::ctime(&now);
        boost::asio::async_write(g::socket, boost::asio::buffer(g::data), write_handler);
    }
}

}

int main(int argc, char* argv[])
{
    g::acceptor.listen();
    std::cout << "[info] time server up" << '\n';
    g::acceptor.async_accept(g::socket, test::accept_handler);
    g::ioservice.run();
    std::cout << "[info] time server down" << '\n';
}
