#include "textbutton.h"

#include "locator.h"
#include "scanconvert.h"

#include <sstream>

using namespace std;
using namespace CapEngine;

TextButton::TextButton(Uint32 windowID, string text, string font, int fontSize,
                       Vector position)
    : TextButton(windowID, text, font, fontSize, position,
                 Colour(0xFF, 0xFF, 0xFF), Colour(0xBA, 0xBA, 0xBA))
{
}

TextButton::TextButton(Uint32 windowID, std::string text, std::string font,
                       int fontSize, CapEngine::Vector position,
                       CapEngine::Colour inactiveColour,
                       CapEngine::Colour activeColour)
    : m_windowID(windowID), m_text(text), m_font(font), m_fontSize(fontSize),
      m_selected(false), m_position(position), m_callback([]() {}),
      m_fontColour(inactiveColour), m_pSelectedTexture(nullptr)
{
    // get surface
    FontManager fontManager;
    Surface *textSurfaceInactive = fontManager.getTextSurface(
        m_font, m_text, m_fontSize, inactiveColour.m_r, inactiveColour.m_g,
        inactiveColour.m_b);
    m_pTextTextureInactive =
        Locator::videoManager->createTextureFromSurface(textSurfaceInactive);
    Locator::videoManager->closeSurface(textSurfaceInactive);

    // Surface* textSurfaceActive = fontManager.getTextSurface(m_font, m_text,
    // m_fontSize, 0xBA, 0xBA, 0xBA);
    Surface *textSurfaceActive =
        fontManager.getTextSurface(m_font, m_text, m_fontSize, activeColour.m_r,
                                   activeColour.m_g, activeColour.m_b);
    m_pTextTextureActive =
        Locator::videoManager->createTextureFromSurface(textSurfaceActive);
    Locator::videoManager->closeSurface(textSurfaceActive);

    // set width and height
    m_width = Locator::videoManager->getTextureWidth(m_pTextTextureInactive);
    m_height = Locator::videoManager->getTextureHeight(m_pTextTextureInactive);

    // Locator::eventDispatcher->subscribe(this, mouseEvent);
    IEventSubscriber::subscribe(Locator::eventDispatcher, mouseEvent);
}

TextButton::~TextButton()
{
    // free surfaces
    Locator::videoManager->closeTexture(m_pTextTextureActive);
    Locator::videoManager->closeTexture(m_pTextTextureInactive);
    if (m_pSelectedTexture != nullptr) {
        Locator::videoManager->closeTexture(m_pSelectedTexture);
    }
    // Locator::eventDispatcher->unsubscribe(this);
}

void TextButton::receiveEvent(const SDL_Event event, CapEngine::Time *time)
{
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        if (mouseInButton(
                Vector(Locator::mouse->getx(), Locator::mouse->gety()))) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                m_activated = true;
            } else { // MOUSEBUTTONUP
                if (m_callback != nullptr) {
                    this->executeCallback();
                }
                m_activated = false;
            }
        } else {
            m_activated = false;
        }
    }
}

void TextButton::update()
{
    if (!(mouseInButton(
            Vector(Locator::mouse->getx(), Locator::mouse->gety())))) {
        m_activated = false;
    }
}

void TextButton::render()
{
    Rect destRect;
    destRect.x = m_position.x;
    destRect.y = m_position.y;
    destRect.w = m_width;
    destRect.h = m_height;
    Rect srcRect;
    srcRect.x = 0.0;
    srcRect.y = 0.0;
    srcRect.w = m_width;
    srcRect.h = m_height;
    // if flag is set,
    if (m_activated) {
        Locator::videoManager->drawTexture(m_windowID, m_pTextTextureActive,
                                           &srcRect, &destRect);
    }
    // if flag is not set
    else {
        Locator::videoManager->drawTexture(m_windowID, m_pTextTextureInactive,
                                           &srcRect, &destRect);
    }

    if (m_selected) {
        // render selected indicator
        // check to see if texture is loaded already and load it if not
        if (m_pSelectedTexture == nullptr) {
            // load a square texture of 75% of the hight of button
            int height = m_height * 0.75;
            int width = height;
            Surface *pSurface =
                CapEngine::createRectangle(width, height, m_fontColour);
            m_pSelectedTexture =
                Locator::videoManager->createTextureFromSurface(pSurface, true);
        }
        Rect rect;
        int w, h;
        Locator::videoManager->getTextureDims(m_pSelectedTexture, &w, &h);
        rect.h = m_height * 0.75;
        rect.w = w * ((double)rect.h / (double)h);
        rect.x = m_position.x - (rect.w * 1.5);
        rect.y = m_position.y + ((m_height - rect.h) / 2);
        Locator::videoManager->drawTexture(m_windowID, m_pSelectedTexture,
                                           nullptr, &rect);
    }
}

int TextButton::getWidth() const { return m_width; }

int TextButton::getHeight() const { return m_height; }

void TextButton::setWidth(int width) { m_width = width; }

void TextButton::setHeight(int height) { m_height = height; }

string TextButton::getText() const { return m_text; }

void TextButton::setText(const string text) { m_text = text; }

void TextButton::setPosition(const Vector position) { m_position = position; }

Vector TextButton::getPosition() const { return m_position; }

void TextButton::registerCallback(void (*callback)(void *), void *context)
{
    // m_callback = callback;
    // m_context = context;
}

void TextButton::registerCallback(std::function<void()> in_callback)
{
    m_callback = in_callback;
}

bool TextButton::mouseInButton(Vector position)
{
    Rectangle buttonRect(m_position.x, m_position.y, m_width, m_height);
    Relation relation = MBRRelate(position.x, position.y, buttonRect);
    if (relation == INSIDE || relation == TOUCH) {
        return true;
    } else {
        return false;
    }
}

void TextButton::setSelected(bool selected) { m_selected = selected; }

bool TextButton::isSelected() const { return m_selected; }

void TextButton::executeCallback() { m_callback(); }

void TextButton::setIndicator(const string imagePath)
{
    try {
        Texture *tempTexture = Locator::videoManager->loadImage(imagePath);
        m_pSelectedTexture = tempTexture;
    } catch (const CapEngineException &e) {
        ostringstream errorStream;
        errorStream << "Unable to load indicator: " << e.what() << endl
                    << "Using default indicator";
        Locator::logger->log(errorStream.str(), Logger::CWARNING, __FILE__,
                             __LINE__);
    }
}
