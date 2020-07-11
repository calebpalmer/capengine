#ifndef CAPENGINE_UIUTILS_H
#define CAPENGINE_UIUTILS_H

#include "property.h"
#include "widgetstate.h"

#include <memory>
#include <vector>

namespace CapEngine
{
namespace UI
{

std::shared_ptr<WidgetState> getWidgetState();
boost::optional<std::shared_ptr<Control>> getCurrentControl();
std::shared_ptr<Widget>
    createPropertyGrid(const std::vector<Property> &properties);

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_UIUTILS_H
