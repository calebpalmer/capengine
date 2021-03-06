#ifndef WINDOWWIDGET_H
#define WINDOWWIDGET_H

#include "colour.h"
#include "widget.h"

#include <boost/signals2.hpp>
#include <functional>
#include <limits>
#include <memory>
#include <string>

namespace CapEngine
{

class EventSubscriber;

namespace UI
{

class WindowWidget : public Widget
{

public:
  static std::shared_ptr<WindowWidget> create(const std::string &name,
                                              int width, int height,
                                              bool resizable = true);

  ~WindowWidget();

  // widget overrides
  virtual SDL_Rect getPosition() const override;
  virtual void setPosition(int x, int y) override;
  virtual void setSize(int width, int height) override;
  virtual void render() override;
  virtual void update(double ms) override;
  virtual std::vector<std::shared_ptr<Widget>> getChildren() override;

  void registerSignals(EventSubscriber &eventSubscriber);
  virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
  virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
  virtual void handleMouseWheelEvent(SDL_MouseWheelEvent event) override;
  virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
  virtual void handleWindowEvent(SDL_WindowEvent event) override;
  virtual void handleTextInputEvent(SDL_TextInputEvent event) override;

  void show();
  void close();
  void setLayout(std::shared_ptr<Widget> pLayout);
  void setMaximized(bool maximized = true);
  void setFullScreen(bool fullScreen = true);

  boost::signals2::scoped_connection
      registerWindowClosedSignal(std::function<void(WindowWidget *)> slot);

private:
  WindowWidget(const std::string &name, int width, int height,
               bool resizable = true);

  void updateSize(int width, int height);

  std::string m_windowName;  //<! The name of the window
  int m_x = 0;               //<! The x position of the window
  int m_y = 0;               //<! The y position of the window
  int m_width = 0;           //<! The width of the window
  int m_height = 0;          //<! The height of the window
  bool m_resizable = true;   //<! flag indicating if the window is resizable
  bool m_maximized = false;  //<! flag indicating if the window is maximized
  bool m_fullScreen = false; //<! flag indicating if window starts full screen
  bool m_shown = false;      //<! flag indicacting if window has been shown yet
  std::shared_ptr<Widget> m_pLayout; //<! The layout of the window
  Colour m_backgroundColour = {150, 150, 150, 255};

  // connections
  //! connection for mouse motion event signals
  boost::signals2::scoped_connection m_handleMouseMotionConnection;
  boost::signals2::scoped_connection m_handleMouseButtonConnection;
  boost::signals2::scoped_connection m_handleMouseWheelConnection;
  boost::signals2::scoped_connection m_handleKeyboardConnection;
  boost::signals2::scoped_connection m_handleWindowConnection;
  boost::signals2::scoped_connection m_handleTextInputConnection;

  // signals
  boost::signals2::signal<void(WindowWidget *)> m_windowClosedSignal;
};

} // namespace UI
} // namespace CapEngine

#endif // WINDOWWIDGET
