#ifndef CAPENGINE_WIDGETDECORATOR_H
#define CAPENGINE_WIDGETDECORATOR_H

#include "widget.h"

namespace CapEngine
{
namespace UI
{

class WidgetDecorator : public Widget
{
public:
  WidgetDecorator(std::shared_ptr<Widget> pWidget);

  virtual ~WidgetDecorator() = default;

  virtual SDL_Rect getPosition() const override;
  virtual void setPosition(int x, int y) override;
  virtual void setSize(int width, int height) override;
  virtual void render() override;
  virtual void update(double ms) override;
  virtual void setParent(Widget *pParent) override;
  virtual void setWindowId(Uint32 windowId) override;
  virtual std::vector<std::shared_ptr<Widget>> getChildren() override;
  virtual bool canFocus() const override;

  virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
  virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
  virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
  virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
  virtual void handleWindowEvent(SDL_WindowEvent event) override;

protected:
  std::shared_ptr<Widget> m_pWidget;
};

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_WIDGETDECORATOR
