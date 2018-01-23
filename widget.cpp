#include "widget.h"

#include "CapEngineException.h"
#include "locator.h"

//! Constructor
/** 
* \param pParentWidget - The parent of this widget.
*/
Widget::Widget(std::shared_ptr<Widget> pParentWidget) :
	m_pParent(pParent) {

	CAP_THROW_NULL(mpParent, "Parent widget is null");
	
	m_windowId = pParentWidget->m_windowId;
	CAP_THROW_ASSERT(m_windowID != kInvalidWindowId, "Parent does not have a window");

	// make sure it's a valid window id
	CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
	if(!Locator::videoManager->isValidWindowId(m_windowId)){
		CAP_THROW(CapEngineException("WindowID is invalid"));
	}
}
