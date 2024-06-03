#include "../include/Log.h"

#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <fstream>
#include <ostream>

void init_logging()
{
    static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

     boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");

    // Output message to console
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = COMMON_FMT,
        boost::log::keywords::auto_flush = true
    );

    // Output message to file, rotates when file reached 1mb or at midnight every day. Each log file
    // is capped at 1mb and total is 20mb
    boost::log::add_file_log (
        boost::log::keywords::file_name = "output.log",
        boost::log::keywords::format = COMMON_FMT,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();

    // Only output message with INFO or higher severity in Release
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );

#if 0
    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;

    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    
    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>("output.log"));
     
    sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

    
  //  auto file_sink = logging::add_console_log();
    
    ptime time(microsec_clock::local_time()); 
    time_duration time_of_day = time.time_of_day(); 
    ptime time2(time - microseconds(time_of_day.total_microseconds()) + hours(time_of_day.hours()) + minutes(time_of_day.minutes()));  
    
    logging::formatter formatter = expr::stream 
            << expr::if_(logging::trivial::severity == logging::trivial::severity_level::info)
            [
               expr::stream << "\033[32m" /*green*/
            ]
            << expr::if_(logging::trivial::severity <= logging::trivial::severity_level::debug)
               [
                     expr::stream << "\033[33m" /*yellow*/
               ]
           .else_
            [
                expr::stream << expr::if_(logging::trivial::severity > logging::trivial::severity_level::info)
               [
                     expr::stream << "\033[31m" /*red*/
               ]
           ]
           << 
    "["<< expr::attr<boost::log::trivial::severity_level>("Severity")<<
    "|"<<time2<< 
    "]"<<"\033[0m"
    << expr::message << "\033[0m";  /*reset*/
    
    
    sink->set_formatter(formatter);
    
    
    sink->set_filter(logging::trivial::severity >= logging::trivial::info );
    
    //file_sink->set_formatter(formatter);
    
    sink->locked_backend()->auto_flush(true);
    
     logging::core::get()->add_sink(sink);
        
//   //  logging::add_common_attributes();
//     
//     logging::core::get()->set_filter
//     (
//         logging::trivial::severity >= logging::trivial::info    /*set the level of Debug you need*/
//        
//     );boost log 
#endif
}
