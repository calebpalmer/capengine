#include "dialogstate.h"

#include "game_management.h"
#include "label.h"
#include "linearlayout.h"
#include "uibutton.h"

namespace CapEngine
{
namespace UI
{

//! Constructor.
/**
 \param msg
   The message to display
 \param callback
   The callback function to call when dialog closes to pass the result.
 \return
*/
DialogState::DialogState(const std::string &msg, callback_t callback)
    : WidgetState(), m_msg(msg), m_callback(callback)
{
}

//! Creates a dialog state.
/**
 \param msg
   The message to display
 \param callback
   The callback function to call when dialog closes to pass the result.
 \return
*/
std::shared_ptr<DialogState> DialogState::create(const std::string &msg,
                                                 callback_t callback)
{
  return std::shared_ptr<DialogState>(new DialogState(msg, callback));
}

//! \copydoc WidgetState::onLoad
bool DialogState::onLoad()
{
  std::shared_ptr<WindowWidget> pWindow =
      this->createWindow(m_kTitle, m_kWidth, m_kHeight);
  pWindow->show();

  std::shared_ptr<LinearLayout> pVerticalLayout =
      LinearLayout::create(LinearLayout::Orientation::Vertical);
  std::shared_ptr<Label> pLabel = Label::create(m_msg);
  pVerticalLayout->addWidget(pLabel);

  std::shared_ptr<LinearLayout> pHorizontalLayout =
      LinearLayout::create(LinearLayout::Orientation::Horizontal);

  std::shared_ptr<Button> pOkButton = Button::create(m_kOkButtonText);
  m_okSignalConnection = pOkButton->addOnButtonClickedHandler([&]() {
    this->m_result = true;
    this->m_done = true;
  });
  pHorizontalLayout->addWidget(pOkButton);

  std::shared_ptr<Button> pCancelButton = Button::create(m_kCancelButtonText);
  m_cancelSignalConnection =
      pCancelButton->addOnButtonClickedHandler([&]() { this->m_done = true; });
  pHorizontalLayout->addWidget(pCancelButton);

  pVerticalLayout->addWidget(pHorizontalLayout);
  pWindow->setLayout(pVerticalLayout);

  return true;
}

//! \copydoc WidgetState::onDestroy
bool DialogState::onDestroy()
{
  m_callback(m_result);
  return true;
}

//! \copydoc WidgetState::update
void DialogState::update(double ms)
{
  WidgetState::update(ms);
  if (m_done) {
    if (std::dynamic_pointer_cast<DialogState>(CapEngine::peekState())) {
      CapEngine::popState();
    }
  }
}

} // namespace UI
} // namespace CapEngine
