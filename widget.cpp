#include "widget.h"

#include "CapEngineException.h"
#include "locator.h"
#include "VideoManager.h"

namespace CapEngine { namespace UI {

//! Constructor
/** 
* \param pParentWidget - The parent of this widget.
*/
Widget::Widget(Widget* pParent) :
	m_pParent(pParent) {

	CAP_THROW_NULL(m_pParent, "Parent widget is null");
	
	m_windowId = m_pParent->m_windowId;
	CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId, "Parent does not have a window");

	// make sure it's a valid window id
	CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
	if(!Locator::videoManager->isValidWindowId(m_windowId)){
		CAP_THROW(CapEngineException("WindowID is invalid"));
	}
}

}} // CapEngine::UI
