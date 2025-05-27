#include "uiutils.h"
#include <boost/log/sources/severity_feature.hpp>

#include "linearlayout.h"
#include "locator.h"
#include "uipropertyvisitor.h"

namespace CapEngine::UI {

//! Gets the WidgetState
/**
 \return - The WidgetState
*/
std::shared_ptr<WidgetState> getWidgetState()
{
    try {
        boost::any maybeWidgetState =
            Locator::locate(WidgetState::kWidgetStateLocatorId);
        auto pWidgetState =
            boost::any_cast<std::shared_ptr<WidgetState>>(maybeWidgetState);

        assert(pWidgetState != nullptr);
        return pWidgetState;
    }
    catch (const boost::bad_any_cast& e) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("Could not locate WidgetState"));
    }
}

//! Get the current active control if there is one.
/**
 \return
   \li The control if there is one.
*/
boost::optional<std::shared_ptr<Control>> getCurrentControl()
{
    std::shared_ptr<WidgetState> pWidgetState = getWidgetState();
    if (pWidgetState == nullptr) return boost::none;

    return pWidgetState->peekControl();
}

//! Creates a Property Grid widget.
/**
 \param properties
   The properties to create the grid from.
 \return
   The Widget holding the properties.
*/
std::shared_ptr<Widget> createPropertyGrid(
    const std::vector<Property>& properties)
{
    auto pGridLayout =
        LinearLayout::create(LinearLayout::Orientation::Vertical);
    assert(pGridLayout != nullptr);

    UIPropertyVisitor visitor;
    for (size_t i = 0; i < properties.size(); i++) {
        Property prop = properties[i];
        prop.accept(visitor);

        std::optional<std::shared_ptr<Widget>> maybePropertyWidget =
            visitor.getWidget();
        assert(maybePropertyWidget != std::nullopt);
        assert(*maybePropertyWidget != nullptr);

        pGridLayout->addWidget(std::move(*maybePropertyWidget));
    }

    return pGridLayout;
}

}  // namespace CapEngine::UI
