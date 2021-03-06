#ifndef CAPENGINE_UIBUTTON_H
#define CAPENGINE_UIBUTTON_H

#include "colour.h"
#include "widget.h"

#include <boost/signals2.hpp>

namespace CapEngine
{
namespace UI
{

//! UI Button class
class Button : public Widget
{
public:
  // represents the state of the button
  enum class State { Neutral, Hovered, Pressed };

public:
  static std::shared_ptr<Button> create(const std::string &text);

  virtual SDL_Rect getPosition() const override;
  virtual void setPosition(int x, int y) override;
  virtual void setSize(int width, int height) override;
  virtual void render() override;
  virtual void update(double ms) override;
  virtual void setWindowId(Uint32 windowId) override;

  virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
  virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;

  boost::signals2::scoped_connection
      addOnButtonClickedHandler(std::function<void()> f);
  boost::signals2::scoped_connection
      addOnButtonHoverHandler(std::function<void()> f);

  std::string getText() const;
  void setText(const std::string &text);

private:
  struct UIConfig {
    std::string fontpath;
    Colour neutralFontColour;
    Colour neutralBackgroundColour;

    Colour hoverFontColour;
    Colour hoverBackgroundColour;

    Colour pressedFontColour;
    Colour pressedBackgroundColour;
  };

private:
  explicit Button(const std::string &text);

  void readUIConfig();
  void updateChildren();
  SDL_Rect getRenderRect() const;

  //! Holds the display configuration
  UIConfig m_uiConfig;
  //! The location and size of the rendering area
  SDL_Rect m_rect;
  //! The text to display
  std::string m_text;
  //! the current state of the button
  State m_state = State::Neutral;
  //! flag indicating whether label position/size needs to be updated
  bool m_updateLabel = true;
  //! The label displaying the text
  std::shared_ptr<Widget> m_pLabel;
  //! flag indicating if button callbacks need to be calledd.
  bool m_doClickCallbacks = false;
  //! signal for button click callbacks
  boost::signals2::signal<void()> m_onButtonClicked;
  //! signal for button hover callbacks
  boost::signals2::signal<void()> m_onButtonHover;
};

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_UIBUTTON_H
