// asio_time_server_coroutine.cpp
//
#include <ctime>
#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/container/list.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace g {

int port = 2345;

boost::asio::io_service ioservice;
boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
boost::asio::ip::tcp::acceptor acceptor(ioservice, endpoint);
#if defined(BOOST_CONTAINER_PERFECT_FORWARDING)
boost::container::list<boost::asio::ip::tcp::socket> sockets;
#else
boost::container::list<boost::shared_ptr<boost::asio::ip::tcp::socket> > socket_ptrs;
#endif

}

namespace test {

void do_write(boost::asio::ip::tcp::socket& socket, boost::asio::yield_context yield)
{
    using namespace boost::asio::ip;

    std::time_t now = std::time(0);
    std::string data = std::ctime(&now);
    boost::asio::async_write(socket, boost::asio::buffer(data), yield);
    socket.shutdown(tcp::socket::shutdown_send);
}

void do_accept(boost::asio::yield_context yield)
{
    using namespace boost::asio::ip;

    const int MAX_CLIENTS = 2;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
#if defined(BOOST_CONTAINER_PERFECT_FORWARDING)
        std::cout << "[hint] perfect forwarding enabled" << '\n';
        g::sockets.emplace_back(g::ioservice);
        tcp::socket& socket = g::sockets.back();
#else
        std::cout << "[hint] perfect forwarding not enabled" << '\n';
        boost::shared_ptr<tcp::socket> ptr(new tcp::socket(g::ioservice));
        g::socket_ptrs.push_back(ptr);
        tcp::socket& socket = *ptr;
#endif
        g::acceptor.async_accept(socket, yield);
        boost::asio::spawn(g::ioservice, boost::bind(&do_write, boost::ref(socket), _1));
    }
}

}

int main(int argc, char* argv[])
{
    g::acceptor.listen();
    boost::asio::spawn(g::ioservice, &test::do_accept);
    g::ioservice.run();
}
