#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <string>

#include "IEventSubscriber.h"
#include "button.h"
#include "captypes.h"
#include "colour.h"

namespace CapEngine {

class TextButton : public Button {
   private:
    TextButton(const TextButton&);
    TextButton& operator=(const TextButton&);

   public:
    TextButton(Uint32 windowID, std::string text, std::string font, int fontSize, CapEngine::Vector position);
    TextButton(Uint32 windowID, std::string text, std::string font, int fontSize, CapEngine::Vector position,
               CapEngine::Colour inactiveColour, CapEngine::Colour activeColour);
    ~TextButton();
    void registerCallback(void (*callback)(void*), void* context) override;
    void registerCallback(std::function<void()> in_function) override;
    void executeCallback();
    void update() override;
    void render() override;
    int getWidth() const override;
    int getHeight() const override;
    void setWidth(int width) override;
    void setHeight(int height) override;
    std::string getText() const override;
    void setText(const std::string text) override;
    void setPosition(const CapEngine::Vector position) override;
    Vector getPosition() const override;
    void setSelected(bool selected) override;
    bool isSelected() const override;
    void setIndicator(const std::string imagePath) override;
    // IEventSubscriber virtual
    void receiveEvent(const SDL_MouseButtonEvent event);

   protected:
    std::string m_text;
    std::string m_font;
    int m_fontSize;
    int m_width;
    int m_height;
    bool m_selected;
    bool m_enabled;
    bool m_activated;
    Uint32 m_windowID;
    CapEngine::Texture* m_pTextTextureInactive;
    CapEngine::Texture* m_pTextTextureActive;
    CapEngine::Texture* m_pSelectedTexture;
    CapEngine::Vector m_position;
    Colour m_fontColour;
    // void (*m_callback)(void *);
    std::function<void()> m_callback;
    void* m_context;
    boost::signals2::scoped_connection m_mouseButtonEventConnection;

   protected:
    bool mouseInButton(CapEngine::Vector position);
};

}  // namespace CapEngine
#endif  // TEXTBUTTON_H
