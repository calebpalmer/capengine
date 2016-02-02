#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <string>

#include "button.h"
#include "IEventSubscriber.h"
#include "colour.h"


namespace CapEngine {

  class TextButton : public Button, public IEventSubscriber {

  private:
    TextButton(const TextButton&);
    TextButton& operator=(const TextButton&);

  public:
    TextButton(std::string text, std::string font, int fontSize, CapEngine::Vector position);
    TextButton(std::string text, std::string font, int fontSize, CapEngine::Vector position, CapEngine::Colour inactiveColour, CapEngine::Colour activeColour);
    ~TextButton();
    void registerCallback(void (*callback)(void*), void* context) override;
    void executeCallback();
    virtual void update() override;
    virtual void render() override;
    virtual void setEnabled(bool enabled = true) override;
    virtual bool isEnabled() const override;
    virtual int getWidth() const override;
    virtual int getHeight() const override;
    virtual void setWidth(int width) override;
    virtual void setHeight(int height) override;
    virtual std::string getText() const override;
    virtual void setText(const std::string text) override;
    virtual void setPosition(const CapEngine::Vector position) override;
    virtual void setSelected(bool selected) override;
    virtual bool isSelected() const override;
    virtual void setIndicator(const std::string imagePath) override;
    // IEventSubscriber virtual
    virtual void receiveEvent(const SDL_Event event, CapEngine::Time* time) override;
    
  protected:
    std::string m_text;
    std::string m_font;
    int m_fontSize;
    int m_width;
    int m_height;
    bool m_selected;
    bool m_enabled;
    bool m_activated;
    CapEngine::Texture* m_pTextTextureInactive;
    CapEngine::Texture* m_pTextTextureActive;
    CapEngine::Texture* m_pSelectedTexture;
    CapEngine::Vector m_position;
    Colour m_fontColour;
    void (*m_callback)(void *);
    void* m_context;

  protected:
    bool mouseInButton(CapEngine::Vector position);
  };

}
#endif // TEXTBUTTON_H
