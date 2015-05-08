// log.cpp
//
#include <exception>
#include <iostream>
#include <iomanip>
#include <string>

#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/exception_handler.hpp>
#include <boost/log/utility/string_literal.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/shared_ptr.hpp>

namespace test {

/// Attribute keyword
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Service", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(counter, "LineCounter", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "Timestamp", boost::posix_time::ptime)

/// Exception handlers
struct log_exception_handler
{
    void operator()(const boost::log::runtime_error& e) const
    {
        std::cerr << "boost::log::runtime_error: " << e.what() << '\n';
    }
    void operator()(const std::exception& e) const
    {
        std::cerr << "std::exception: " << e.what() << '\n';
    }
};

/// Global logger
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger, boost::log::sources::wlogger_mt);

}

int main(int argc, char* argv[])
{
    using namespace boost::log;

    core::get()->set_exception_handler(
            make_exception_handler<boost::log::runtime_error, std::exception>(
                test::log_exception_handler()));

    /// [1]
    typedef sinks::asynchronous_sink<sinks::text_ostream_backend> ostream_sink;
    boost::shared_ptr<ostream_sink> stream = boost::make_shared<ostream_sink>();
    boost::shared_ptr<std::ostream> ostream(&std::cout, boost::empty_deleter());
    stream->locked_backend()->add_stream(ostream);
    stream->set_filter(expressions::is_in_range(test::severity, 1, 3));
    stream->set_formatter(expressions::stream
            << std::setw(5) << test::counter << " - "
            << test::severity << ": " << expressions::smessage << "("
            << expressions::format_date_time(test::timestamp, "%H:%M:%S") << ")");

    core::get()->add_sink(stream);
    core::get()->add_global_attribute("LineCounter", attributes::counter<int>());

    /// [2]
    typedef sinks::synchronous_sink<sinks::text_multifile_backend> multifile_sink;
    boost::shared_ptr<multifile_sink> multifile = boost::make_shared<multifile_sink>();
    multifile->locked_backend()->set_file_name_composer(
            sinks::file::as_file_name_composer(expressions::stream
                << test::channel.or_default<std::string>("None") << "-"
                << test::severity.or_default<int>(0) << ".log"));
    core::get()->add_sink(multifile);

    sources::severity_logger<int> severity_lg;
    sources::channel_logger<> channel_lg(keywords::channel = "Main");

    BOOST_LOG(channel_lg) << "channel message";
    BOOST_LOG_SEV(severity_lg, 1) << "severity message";
    do {
        BOOST_LOG_SCOPED_LOGGER_ATTR(severity_lg, "Timestamp", attributes::local_clock());
        BOOST_LOG_SEV(severity_lg, 2) << "error";
    } while (0);
    BOOST_LOG_SEV(severity_lg, 2) << "another error";
    BOOST_LOG(test::logger::get()) << L"note";

    stream->flush();
}
