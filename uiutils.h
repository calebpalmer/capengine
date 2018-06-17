#ifndef CAPENGINE_UIUTILS_H
#define CAPENGINE_UIUTILS_H

#include "widgetstate.h"

#include <vector>
#include <memory>

namespace CapEngine { namespace UI {

std::shared_ptr<WidgetState> getWidgetState();
boost::optional<std::shared_ptr<Control>> getCurrentControl();

}}

#endif // CAPENGINE_UIUTILS_H
