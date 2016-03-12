#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include <memory>
#include <vector>

#include "uiobject.h"
#include "button.h"
#include "IEventSubscriber.h"

namespace CapEngine{
  class ButtonGroup : public UIObject, public IEventSubscriber {
  public:
    ButtonGroup();
    virtual ~ButtonGroup();

    // UIObject virtual inherited members
    virtual void update() override;
    virtual void render() override;
    // These following few don't do anything
    virtual void setEnabled(bool enabled) override {}
    virtual bool isEnabled() const override {return true;}
    virtual int getWidth() const override {return -1;}
    virtual int getHeight() const override {return -1;}
    virtual void setWidth(int width) override {}
    virtual void setHeight(int height) override {}
    virtual void setPosition(const Vector position) override {}
    virtual void setSelected(bool selected) override {}
    virtual bool isSelected() const override {return false;}

    // IEventSubscriber virtual inherited members
    virtual void receiveEvent(const SDL_Event event, CapEngine::Time* time) override; 

    virtual void addButton(std::unique_ptr<Button>);
    virtual void removeButton(int index);

  protected:
    ButtonGroup(const ButtonGroup&);
    Button& operator=(const ButtonGroup&);

    std::vector<std::unique_ptr<Button>> m_buttons;
    unsigned int m_activeButtonIndex;
  };
}

#endif // BUTTON_GROUP_H
