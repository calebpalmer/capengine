#ifndef CAPENGINE_LOGGING_H
#define CAPENGINE_LOGGING_H

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/signals2.hpp>

namespace CapEngine {

extern boost::log::sources::severity_logger<boost::log::trivial::severity_level> log;

void initLogging();
void logException(const std::exception& e);
}

#endif // CAPENGINE_LOGGING_H
