#include "label.h"

#include "CapEngineException.h"
#include "VideoManager.h"
#include "locator.h"
#include "uiconfigmanager.h"
#include "utils.h"

namespace CapEngine
{
namespace UI
{

Label::Label(const std::string &text, std::optional<std::string> font,
             std::optional<int> fontSize, std::optional<Colour> colour)
    : m_text(text), m_texture(getNullTexturePtr())
{
    if (font == std::nullopt || font->empty()) {
        boost::optional<jsoncons::json> maybeFont =
            UIConfigManager::getInstance().getSetting(kDefaultFontSettingsPath);
        if (!maybeFont) {
            CAP_THROW(CapEngineException("Default font setting not found"));
        }

        m_font = (*maybeFont)[0].as<std::string>();
    } else {
        m_font = *font;
    }

    if (font == std::nullopt) {
        boost::optional<jsoncons::json> maybeFontSize =
            UIConfigManager::getInstance().getSetting(
                kDefaultFontSizeSettingsPath);

        if (maybeFontSize)
            m_fontSize = (*maybeFontSize)[0].as<int>();
        else
            m_fontSize = kDefaultFontSize;
    } else {
        m_fontSize = *fontSize;
    }

    if (colour == std::nullopt) {
        m_colour = Colour(0, 0, 0);
    } else {
        m_colour = *colour;
    }
}

std::shared_ptr<Label> Label::create(const std::string &text,
                                     std::optional<std::string> font,
                                     std::optional<int> fontSize,
                                     std::optional<Colour> colour)
{
    std::shared_ptr<Label> pLabel(new Label(text, font, fontSize, colour));
    return pLabel;
}

//! @copydoc Widget::setPosition()
void Label::setPosition(int x, int y)
{
  m_x = x;
  m_y = y;
}

//! @copydoc Widget::setSize()
void Label::setSize(int width, int height)
{
    if (width != m_width || height != m_height) {
        m_width = width;
        m_height = height;
    }
}

void Label::loadTexture()
{
    // generate the texture
    CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
    CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId,
                     "Invalid window id");

    SDL_Surface *surface =
        m_fontManager.getTextSurface(m_font, m_text, m_fontSize, m_colour);

    m_texture = Locator::videoManager->createTextureFromSurfacePtr(
        m_windowId, surface, true);
    CAP_THROW_ASSERT(m_texture != nullptr, "Unable to create texture");
}

std::pair<int, int> Label::getTextureSize()
{
    if (m_texture == nullptr) {
        loadTexture();
    }

    int textureWidth = 0;
    int textureHeight = 0;
    Locator::videoManager->getTextureDims(m_texture.get(), &textureWidth,
                                          &textureHeight);

    return std::make_pair(textureWidth, textureHeight);
}

//! @copydoc Widget::render()
void Label::render()
{
    CAP_THROW_ASSERT(Locator::videoManager != nullptr, "VideoManager is null");
    CAP_THROW_ASSERT(m_windowId != VideoManager::kInvalidWindowId,
                     "Invalid window id");

    if (m_texture == nullptr) {
        loadTexture();
    }

    auto [textureWidth, textureHeight] = this->getTextureSize();

    SDL_Rect srcRect = {0, 0, textureWidth, textureHeight};
    // TODO I don't know why it was done like this but the below fixed
    // rendering issues I was having. Leaving this here in case.
    // SDL_Rect dstRect = {
    //     textureWidth < m_width ? m_x + ((m_width - textureWidth) / 2)
    //                            : m_x, // x
    //     textureHeight < m_height ? m_y + ((m_height - textureHeight) / 2)
    //                              : m_y,                     // y
    //     textureWidth < m_width ? textureWidth : m_width,    // width
    //     textureHeight < m_height ? textureHeight : m_height // height
    // };

    SDL_Rect dstRect = {m_x, m_y, m_width, m_height};

    // left alignment
    if (m_horizontalAlignment == HorizontalAlignment::Right &&
        textureWidth < m_width) {
        dstRect.x = m_x + m_width - textureWidth;
    }

    // right alignment
    if (m_horizontalAlignment == HorizontalAlignment::Left &&
        textureWidth < m_width) {
        dstRect.x = m_x;
    }

    // top alignment
    if (m_verticalAlignment == VerticalAlignment::Top &&
        textureHeight < m_height) {
        dstRect.y = m_y;
    }

    // bottom alignment
    if (m_verticalAlignment == VerticalAlignment::Bottom &&
        textureHeight < m_height) {
        dstRect.y = m_y + m_height - textureHeight;
    }

    Locator::videoManager->drawTexture(m_windowId, m_texture.get(), &srcRect,
                                       &dstRect);
}

//! Gets the labels text
/**
 \return - The text.
*/
std::string Label::getText() const { return m_text; }

//! Sets the text of the label.
/**
 \param text - The text to set.
*/
void Label::setText(const std::string &text)
{
  m_text = text;
  m_texture.release();
}

//! \copydoc Widget::getPosition
SDL_Rect Label::getPosition() const { return {m_x, m_y, m_width, m_height}; }

} // namespace UI
} // namespace CapEngine
