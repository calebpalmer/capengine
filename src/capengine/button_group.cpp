#include "button_group.h"

#include <SDL2/SDL_events.h>
#include <assert.h>

#include <cmath>
#include <sstream>

#include "locator.h"
#include "logging.h"

using namespace CapEngine;
using namespace std;

ButtonGroup::ButtonGroup() : m_activeButtonIndex(0)
{
    assert(Locator::eventSubscriber != nullptr);
    m_keyboardEventConnection = Locator::eventSubscriber->m_keyboardEventSignal.connect(
        [this](SDL_KeyboardEvent in_keyboardEvent) { this->handleKeyboardEvent(in_keyboardEvent); });
}

ButtonGroup::~ButtonGroup()
{
    // Locator::eventDispatcher->unsubscribe(this);
}

void ButtonGroup::update()
{
    for (unsigned int i = 0; i < m_buttons.size(); i++) {
        m_buttons[i]->update();
        if (i == m_activeButtonIndex) {
            m_buttons[i]->setSelected(true);
        } else {
            m_buttons[i]->setSelected(false);
        }
    }
}

void ButtonGroup::render()
{
    for (auto &i : m_buttons) {
        i->render();
    }
}

void ButtonGroup::handleKeyboardEvent(SDL_KeyboardEvent in_keyboardEvent)
{
    if (m_enabled) {
        // keyboard
        if (in_keyboardEvent.keysym.sym == SDLK_UP && in_keyboardEvent.type == SDL_KEYDOWN) {
            ostringstream msg;
            m_activeButtonIndex =
                static_cast<int>(std::abs(static_cast<double>(m_activeButtonIndex - 1))) % m_buttons.size();
            msg << "Button group index changed to " << m_activeButtonIndex;
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << msg.str();
        }
        if (in_keyboardEvent.keysym.sym == SDLK_DOWN && in_keyboardEvent.type == SDL_KEYUP) {
            ostringstream msg;
            m_activeButtonIndex = (m_activeButtonIndex + 1) % m_buttons.size();
            msg << "Button group index changed to " << m_activeButtonIndex;
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << msg.str();
        }
        // if enter, activate current button
        if (in_keyboardEvent.keysym.sym == SDLK_RETURN) {
            m_buttons[m_activeButtonIndex]->executeCallback();
        }
    }
}

void ButtonGroup::addButton(std::unique_ptr<Button> pbutton) { m_buttons.push_back(std::move(pbutton)); }

void ButtonGroup::removeButton(int index) {}

void ButtonGroup::listenController(shared_ptr<Controller> pController)
{
  m_controllers.push_back(pController);
}

void ButtonGroup::forgetController(shared_ptr<Controller> pController)
{
    // doesn't handle duplicates
    auto i = m_controllers.begin();
    for (; i != m_controllers.end(); i++) {
        if ((*i)->getId() == pController->getId()) {
            break;
        }
    }
    if (i != m_controllers.end()) {
        m_controllers.erase(i);
    }
}
