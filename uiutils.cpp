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


//! Get the current active control if there is one.
/** 
 \return
   \li The control if there is one.
*/
boost::optional<std::shared_ptr<Control>> getCurrentControl(){
	std::shared_ptr<WidgetState> pWidgetState = getWidgetState();
	if(pWidgetState == nullptr)
		return boost::none;

	return pWidgetState->peekControl();

	
}

}}
