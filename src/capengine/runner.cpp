#include "runner.h"

#include <boost/log/trivial.hpp>
#include <sstream>

#include "CapEngineException.h"
#include "filesystem.h"
#include "game_management.h"
#include "locator.h"
#include "logging.h"
#include "widget.h"

namespace CapEngine {

Runner::Runner() : m_quit(false), m_msPerUpdate(16.67), m_showFPS(false) {}

Runner* Runner::s_pRunner = nullptr;

Runner& Runner::getInstance()
{
    if (s_pRunner == nullptr) {
        s_pRunner = new Runner();
    }
    return *s_pRunner;
}

void Runner::popState(bool resumePrevious)
{
    if (m_gameStates.size() > 0) {
        auto pPoppedState = std::move(m_gameStates.back());
        m_gameStates.pop_back();
        if (pPoppedState->onDestroy() == false) {
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning) << "Failed to destroy popped state";
        }
        // toss it in the trash to be removed i the next update
        m_stateTrash.push_back(std::move(pPoppedState));

        if (resumePrevious) {
            if (m_gameStates.size() == 0)
                m_quit = true;
            else {
                CAP_THROW_ASSERT(m_gameStates.back()->onResume(), "Unable to resume state.");
            }
        }
    }
}

void Runner::pushState(std::shared_ptr<GameState> pGameState)
{
    if (m_gameStates.size() > 0) CAP_THROW_ASSERT(m_gameStates.back()->onPause(), "Error pausing state.");

    m_gameStates.push_back(std::move(pGameState));
    if (m_gameStates.back()->onLoad() == false) {
        BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning) << "Failed to init pushed state";
    }
}

//! Get the state at the top of the stack.
/**
 \return
   The GameState if there is one.  null if their isn't.
*/
std::shared_ptr<GameState> Runner::peekState()
{
    if (m_gameStates.size() > 0) return m_gameStates.back();

    return std::shared_ptr<GameState>();
}

void Runner::switchState(std::shared_ptr<GameState> pGameState)
{
    while (!m_gameStates.empty()) {
        this->popState(false);
    }
    this->pushState(std::move(pGameState));
}

void Runner::end() { m_quit = true; }

void Runner::loop()
{
    assert(Locator::eventDispatcher != nullptr);

    int subscriptionMask = mouseEvent | keyboardEvent | systemEvent | windowEvent;
    // Locator::eventDispatcher->subscribe(this, subscriptionMask);
    IEventSubscriber::subscribe(Locator::eventDispatcher, subscriptionMask);
    double previous = currentTime();
    double lag = 0.0;
    while (!m_quit) {
        double current = currentTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        // process input
        Locator::eventDispatcher->getEvents();
        if (Locator::eventDispatcher->hasEvents()) {
            Locator::eventDispatcher->flushQueue();
        }

        while (lag >= m_msPerUpdate) {
            update();
            lag -= m_msPerUpdate;
        }
        render(1.0);
    }
    CapEngine::destroy();
}

void Runner::exit() { m_quit = true; }

void Runner::update()
{
    // clean up the trash
    m_stateTrash.clear();

    if (m_gameStates.size() > 0) (m_gameStates.back())->update(m_msPerUpdate);

    // update the keyboard state.
    if (Locator::keyboard) Locator::keyboard->update();

    // update any Widgets (usually WindowWidgets) if there are any
    for (auto&& pWidget : m_widgets) {
        pWidget->update(m_msPerUpdate);
    }
}

void Runner::render(double /*frameFactor*/)
{
    Locator::videoManager->clearAll();

    for (auto&& pGameState : m_gameStates) pGameState->render();

    Locator::videoManager->drawAll();
}

void Runner::receiveEvent(const SDL_Event event, CapEngine::Time* /*time*/)
{
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE && m_defaultQuitEventsEnabled) {
            m_quit = true;
        }
    }

    // exit when 'q' is pressed
    if ((event.type == SDL_KEYUP && ((SDL_KeyboardEvent*)&event)->keysym.sym == SDLK_q)) {
        if (m_defaultQuitEventsEnabled) {
            m_quit = true;
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "quitting";
            return;
        }
    }

    // update the mouse position
    else if (event.type == SDL_MOUSEMOTION) {
        int x = ((SDL_MouseMotionEvent*)&event)->x;
        int y = ((SDL_MouseMotionEvent*)&event)->y;
        Locator::mouse->setx(x);
        Locator::mouse->sety(y);
    }
    // update the mouse buttons
    else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        if (((SDL_MouseButtonEvent*)&event)->type == SDL_MOUSEBUTTONDOWN) {
            switch (((SDL_MouseButtonEvent*)&event)->button) {
                case SDL_BUTTON_LEFT:
                    Locator::mouse->setButtonState(0, true);
                    break;
                case SDL_BUTTON_MIDDLE:
                    Locator::mouse->setButtonState(1, true);
                    break;
                case SDL_BUTTON_RIGHT:
                    Locator::mouse->setButtonState(2, true);
                    break;
            }
        }
        if (((SDL_MouseButtonEvent*)&event)->type == SDL_MOUSEBUTTONUP) {
            switch (((SDL_MouseButtonEvent*)&event)->button) {
                case SDL_BUTTON_LEFT:
                    Locator::mouse->setButtonState(0, false);
                    break;
                case SDL_BUTTON_MIDDLE:
                    Locator::mouse->setButtonState(1, false);
                    break;
                case SDL_BUTTON_RIGHT:
                    Locator::mouse->setButtonState(2, false);
                    break;
            }
        }
    }
    // check to see if fps should be shown
    if (event.type == SDL_KEYUP) {
        SDL_Keycode ksym = ((SDL_KeyboardEvent*)&event)->keysym.sym;
        if (m_defaultQuitEventsEnabled) {
            if (ksym == SDLK_TAB) {
                if (m_showFPS == true) {
                    m_showFPS = false;
                    Locator::videoManager->displayFPS(false);
                }
                else {
                    m_showFPS = true;
                    std::ostringstream ttfStream;
                    ttfStream << getCurrentDir() << "/res/fonts/tahoma.ttf";
                    Uint8 r = 255;
                    Uint8 g = 255;
                    Uint8 b = 255;
                    Locator::videoManager->displayFPS(true, ttfStream.str(), r, g, b);
                }
            }
        }
    }
}

//! Enables/Disables default quit events (window close and q key)
/**
 \param enabled
   if true then enabled, false then disabled.
*/
void Runner::setDefaultQuitEvents(bool enabled) { m_defaultQuitEventsEnabled = enabled; }

}  // namespace CapEngine
