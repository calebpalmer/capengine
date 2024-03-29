#include "CapEngineException.h"
#include "VideoManager.h"
#include "absolutelayout.h"
#include "game_management.h"
#include "gamestate.h"
#include "gridlayout.h"
#include "label.h"
#include "linearlayout.h"
#include "locator.h"
#include "orientable.h"
#include "textbox.h"
#include "uibutton.h"
#include "widgetstate.h"
#include "windowwidget.h"

#include <functional>
#include <iostream>
#include <memory>

namespace
{

using namespace CapEngine;

void onButtonClicked(std::shared_ptr<UI::Label> pLabel)
{
  assert(pLabel);
  pLabel->setText("changed by button click");
}

bool onLoad(CapEngine::UI::WidgetState &widgetState)
{
    std::string name = "Widget Test";
    int width = 640;
    int height = 480;
    auto pWindow = widgetState.createWindow(name, width, height);
    pWindow->show();

    std::shared_ptr<UI::GridLayout> pGridLayout = UI::GridLayout::create(2, 2);
    pWindow->setLayout(pGridLayout);

    auto pLabel = UI::Label::create("test");
    pGridLayout->addWidget(pLabel, 1, 0);

    std::shared_ptr<UI::LinearLayout> pLinearLayout = UI::LinearLayout::create(
        CapEngine::UI::LinearLayout::Orientation::Horizontal);

    std::shared_ptr<UI::Label> pLabelLeft = UI::Label::create("Label");
    pLabelLeft->setHorizontalAlignment(
        UI::Orientable::HorizontalAlignment::Right);

    std::shared_ptr<UI::Button> pButton = UI::Button::create("Press Me");
    static boost::signals2::scoped_connection connection =
        pButton->addOnButtonClickedHandler(
            std::bind(onButtonClicked, pLabelLeft));

    pLinearLayout->addWidget(pLabelLeft);
    pLinearLayout->addWidget(pButton);

    pGridLayout->addWidget(pLinearLayout, 0, 1);

    auto pTextBox = UI::TextBox::create("This is a textbox");
    pGridLayout->addWidget(pTextBox, 1, 1);

    pWindow->setLayout(pGridLayout);

    // auto pAbsLayout = UI::AbsoluteLayout::create();
    // pWindow->setLayout(pAbsLayout);

    // // layout for the top
    // auto pLinearLayout = UI::LinearLayout::create();
    // pAbsLayout->addWidget(pLinearLayout, 0, 0, 100, 30, UI::Unit::Percentage,
    // UI::Unit::Pixels);

    // auto pLabel = UI::Label::create("I'm at the top");
    // pLinearLayout->addWidget(pLabel);

    return true;
}

bool onDestroy(CapEngine::UI::WidgetState & /*widgetState*/) { return true; }

} // anonymous namespace

int main()
{
    using namespace CapEngine;

    try {
        WindowParams params;
        CapEngine::init(params, true);

        std::shared_ptr<UI::WidgetState> pWidgetTestState =
            UI::WidgetState::create(onLoad, onDestroy);
        CapEngine::startLoop(std::move(pWidgetTestState));
    } catch (const boost::exception &e) {
        std::cerr << boost::diagnostic_information(e);
    }
}
