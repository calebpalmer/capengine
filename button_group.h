#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include <memory>
#include <vector>

#include "IEventSubscriber.h"
#include "button.h"
#include "controller.h"
#include "uiobject.h"

namespace CapEngine
{
class ButtonGroup : public UIObject, public IEventSubscriber
{
  public:
    ButtonGroup();
    ~ButtonGroup() override;

    // UIObject inherited members
    void update() override;
    void render() override;
    int getWidth() const override { return -1; }
    int getHeight() const override { return -1; }
    void setWidth(int width) override {}
    void setHeight(int height) override {}
    void setPosition(const Vector position) override {}
    Vector getPosition() const override { return Vector{}; };

    void setSelected(bool selected) override {}
    bool isSelected() const override { return false; }

    // IEventSubscriber inherited members
    void receiveEvent(const SDL_Event event, CapEngine::Time *time) override;

    virtual void addButton(std::unique_ptr<Button>);
    virtual void removeButton(int index);
    virtual void listenController(std::shared_ptr<Controller> pController);
    virtual void forgetController(std::shared_ptr<Controller> pController);

  protected:
    ButtonGroup(const ButtonGroup &);
    Button &operator=(const ButtonGroup &);

    std::vector<std::unique_ptr<Button>> m_buttons;
    unsigned int m_activeButtonIndex;
    std::vector<std::shared_ptr<Controller>> m_controllers;
};
} // namespace CapEngine

#endif // BUTTON_GROUP_H
