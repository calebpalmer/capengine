#include "editorutils.h"

#include "widgetstate.h"
#include "locator.h"

namespace CapEngine { namespace UI {

//! Gets the control stack
/** 
 \return - The control stack
*/
std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>> getControlStack(){
	boost::any maybeControlStack = Locator::locate(WidgetState::kControlStackId);

	std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>> pControlStack;
	try{
		pControlStack = boost::any_cast<std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>>>(maybeControlStack);
	}
	catch(const boost::bad_any_cast &e){
		assert(Locator::logger != nullptr);
		Locator::logger->log(e, Logger::CWARNING, __FILE__, __LINE__);
		BOOST_THROW_EXCEPTION(CapEngineException("Could not locate control stack"));
	}

	assert(pControlStack != nullptr);
	return pControlStack;
}

}}
