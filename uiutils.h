#ifndef CAPENGINE_UIUTILS_H
#define CAPENGINE_UIUTILS_H

#include "widgetstate.h"
#include "property.h"

#include <vector>
#include <memory>

namespace CapEngine { namespace UI {

std::shared_ptr<WidgetState> getWidgetState();
boost::optional<std::shared_ptr<Control>> getCurrentControl();
std::shared_ptr<Widget> createPropertyGrid(const std::vector<Property> &properties);

}}

#endif // CAPENGINE_UIUTILS_H
