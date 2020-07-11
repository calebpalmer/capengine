#include "linearlayout.h"

namespace CapEngine
{
namespace UI
{

//! Constructo
/**
 \param orientation - The orientation of the layout
*/
LinearLayout::LinearLayout(Orientation orientation)
{
  m_orientation = orientation;
}

//! Creates a LinearLayout
/**
 \return the LinearLayout
*/
std::shared_ptr<LinearLayout> LinearLayout::create(Orientation orientation)
{
  return std::shared_ptr<LinearLayout>(new LinearLayout(orientation));
}

//! \copydoc Widget::setPosition
void LinearLayout::setPosition(int x, int y)
{
  m_rect.x = x;
  m_rect.y = y;

  m_updateChildren = true;
}

//! \copydoc Widget::setSize
void LinearLayout::setSize(int width, int height)
{
  m_rect.w = width;
  m_rect.h = height;

  m_updateChildren = true;
}

//! \copydoc Widget::render
void LinearLayout::render()
{
  if (m_updateChildren)
    this->updateChildren();

  for (auto &&pWidget : m_widgets) {
    CAP_THROW_NULL(pWidget, "Widget is null");
    pWidget->render();
  }
}

//! \copydoc Widget::update
void LinearLayout::update(double ms)
{
  for (auto &&pWidget : m_widgets) {
    CAP_THROW_NULL(pWidget, "Widget is null");
    pWidget->update(ms);
  }
}

//! \copydoc Widget::setWindowId
void LinearLayout::setWindowId(Uint32 windowId)
{
  Widget::setWindowId(windowId);

  for (auto &&pWidget : m_widgets) {
    CAP_THROW_NULL(pWidget, "Widget is null");
    pWidget->setWindowId(windowId);
  }
}

//! Adds a widget to the layout
/**
 \param pWidget - The widget to add.
*/
void LinearLayout::addWidget(std::shared_ptr<Widget> pWidget)
{
  CAP_THROW_NULL(pWidget, "The widget is null");
  m_widgets.push_back(pWidget);

  pWidget->setParent(this);
  pWidget->setWindowId(this->m_windowId);

  m_updateChildren = true;
}

//! update size and position of children
void LinearLayout::updateChildren()
{
  int widgetWidth = m_orientation == Orientation::Horizontal
                        ? m_rect.w / m_widgets.size()
                        : m_rect.w;

  int widgetHeight = m_orientation == Orientation::Vertical
                         ? m_rect.h / m_widgets.size()
                         : m_rect.h;

  for (size_t i = 0; i < m_widgets.size(); i++) {
    CAP_THROW_NULL(m_widgets[i], "Widget is null");
    m_widgets[i]->setSize(widgetWidth, widgetHeight);

    if (m_orientation == Orientation::Horizontal) {
      switch (m_alignment) {
      case Alignment::Begin:
        break;
      case Alignment::Center:
        m_widgets[i]->setPosition(m_rect.x + i * widgetWidth, m_rect.y);
        break;
      case Alignment::End:
        break;
      }
    } else {
      m_widgets[i]->setPosition(m_rect.x, m_rect.y + i * widgetHeight);
    }
  }

  m_updateChildren = false;
}

//! \copydoc Widget::handleMouseMotionEvent
void LinearLayout::handleMouseMotionEvent(SDL_MouseMotionEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleMouseMotionEvent(event);
  }
}

//! \copydoc Widget::handleMouseButtonEvent
void LinearLayout::handleMouseButtonEvent(SDL_MouseButtonEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleMouseButtonEvent(event);
  }
}

//! \copydoc Widget::handleMouseWheelEvent
void LinearLayout::handleMouseWheelEvent(SDL_MouseWheelEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleMouseWheelEvent(event);
  }
}

//! \copydoc Widget::handleKeyboardEvent
void LinearLayout::handleKeyboardEvent(SDL_KeyboardEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleKeyboardEvent(event);
  }
}

//! \copydoc Widget::handleWindowEvent
void LinearLayout::handleWindowEvent(SDL_WindowEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleWindowEvent(event);
  }
}

//! \copydoc Widget::handleTextInputEvent
void LinearLayout::handleTextInputEvent(SDL_TextInputEvent event)
{
  for (auto &&pWidget : m_widgets) {
    pWidget->handleTextInputEvent(event);
  }
}

//! Widget:getChildren
std::vector<std::shared_ptr<Widget>> LinearLayout::getChildren()
{
  return m_widgets;
}

//! \copydoc Widget::getPosition
SDL_Rect LinearLayout::getPosition() const { return m_rect; }

//! Gets the alignment.
/**
 \return
   The alignment
*/
LinearLayout::Alignment LinearLayout::getAlignment() const
{
  return m_alignment;
}

//! Sets the alignment.
/**
 \param alignment
   The alignment
*/
void LinearLayout::setAlignment(Alignment alignment)
{
  m_alignment = alignment;
}

} // namespace UI
} // namespace CapEngine
