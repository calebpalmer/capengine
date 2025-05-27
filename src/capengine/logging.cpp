#include "logging.h"

#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace CapEngine {

boost::log::sources::severity_logger<boost::log::trivial::severity_level> log;

void initLogging()
{
    boost::log::add_console_log(std::clog, boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%");
    boost::log::add_common_attributes();
}

void logException(const std::exception& e)
{
    BOOST_LOG_SEV(log, boost::log::trivial::error) << "Exception: " << e.what();
    if (const auto* be = dynamic_cast<const boost::exception*>(&e)) {
        BOOST_LOG_SEV(log, boost::log::trivial::error) << boost::diagnostic_information(*be);
    }
}

}  // namespace CapEngine
