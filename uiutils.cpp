#include "uiutils.h"

#include "locator.h"

namespace CapEngine { namespace UI {

//! Gets the WidgetState
/** 
 \return - The WidgetState
*/
std::shared_ptr<WidgetState> getWidgetState(){
	try {
		boost::any maybeWidgetState = Locator::locate(WidgetState::kWidgetStateLocatorId);
		auto pWidgetState = boost::any_cast<std::shared_ptr<WidgetState>>(maybeWidgetState);

		assert(pWidgetState != nullptr);
		return pWidgetState;
	}
	catch(const boost::bad_any_cast &e){
		assert(Locator::logger != nullptr);
		Locator::logger->log(e, Logger::CWARNING, __FILE__, __LINE__);
		BOOST_THROW_EXCEPTION(CapEngineException("Could not locate WidgetState"));
	}
}

}}
