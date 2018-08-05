#include "textbox.h"

#include "collision.h"
#include "uiconfigmanager.h"
#include "logger.h"
#include "locator.h"

#include <boost/format.hpp>
#include <iostream> // TODO remove me

namespace CapEngine { namespace UI {

namespace {

const char kTextboxProperty[] = "textbox";
const char kBackgroundColourProperty[] = "background-colour";
const char kFontColourProperty[] = "font-colour";
const char kColourProperty[] = "colour";

} // namespace

const Colour TextBox::kDefaultFontColour{0, 0, 0, 255};
const Colour TextBox::kDefaultBackgroundColour{255, 255, 255, 255};

//! Constructor
/** 
 \param initialText
   \li any initial text to put in the textbox.
*/
TextBox::TextBox(std::string initialText)
	: m_text(std::move(initialText)), m_texture(getNullTexturePtr()),
		m_displaySettings(getDisplaySettings()),
		m_font(Font(m_displaySettings.fontPath, m_displaySettings.fontSize))
{
	
}

//! Creates a textbox
/** 
 \param initialText
   \li initial text to put in the textbox.
	\return 
	  \li The TextBox
*/
std::shared_ptr<TextBox> TextBox::create(std::string initialText){
	return std::shared_ptr<TextBox>(new TextBox(std::move(initialText)));
}


//! \copydoc Widget::getPosition
SDL_Rect TextBox::getPosition() const {
	return m_rect;
}

//! \copydoc Widget::setPosition
void TextBox::setPosition(int x, int y) {
	m_rect.x = x;
	m_rect.y = y;
	m_textureDirty = true;
}

//! \copydoc Widget::setSize
void TextBox::setSize(int width, int height) {
	m_rect.w = width;
	m_rect.h = height;
	m_textureDirty = true;
}

//! \copydoc Widget::render
void TextBox::render() {
	if(m_textureDirty)
		this->updateTexture();

	assert(Locator::videoManager != nullptr);
	Locator::videoManager->drawFillRect(m_windowId, m_boxRect, m_displaySettings.backgroundColour);
}

//! \copydoc Widget::canFocus
bool TextBox::canFocus() const {
	return true;
}

//! \copydoc Widget::doFocus
bool TextBox::doFocus(bool focus, int downX, int downY, int upX, int upY) {
	if(focus == false) {
		m_hasFocus = false;
		return false;
	}

	else {
		if(pointInRect( { downX, downY }, m_rect) && pointInRect( { upX, upY }, m_rect)){
			m_hasFocus = true;
			return true;
		}

		return false;
	}
}

//! Updates the texture that is displayed.
void TextBox::updateTexture() {
	// get the height of the textbox based on the font
	m_boxRect.w = m_rect.w;
	m_boxRect.h = m_font.getFontHeight();
	m_boxRect.x = m_rect.x;
	m_boxRect.y = (m_rect.y + (m_rect.h / 2)) - (m_boxRect.h / 2);


	// render the text

	m_textureDirty = false;
}

//! loads the display settings to use.
TextBox::DisplaySettings TextBox::getDisplaySettings(){
	DisplaySettings settings;
	UIConfigManager &configManager = UIConfigManager::getInstance();
	
	// get the font colour
	const std::string fontColourPath =
		(boost::format("$.%1%.%2%.%3%") % kTextboxProperty % kFontColourProperty % kColourProperty).str();

	boost::optional<jsoncons::json> maybeFontColourJson = configManager.getSetting(fontColourPath);
	
	if(maybeFontColourJson != boost::none){
		boost::optional<Colour> maybeColour = getColour(*maybeFontColourJson);

		if(maybeColour != boost::none)
			settings.fontColour = *maybeColour;
	}

	// get the font size
	boost::optional<jsoncons::json> maybeFontSizeJson = configManager.getSetting(kDefaultFontSizeSettingsPath);
	if(maybeFontSizeJson != boost::none){
		std::cout << jsoncons::print(*maybeFontSizeJson) << std::endl;
		settings.fontSize = (*maybeFontSizeJson)[0].as<int>();
	}

	// get the font path
	boost::optional<jsoncons::json> maybeFont = configManager.getSetting(kDefaultFontSettingsPath);
	if(maybeFont == boost::none){
		BOOST_THROW_EXCEPTION(CapEngineException("No default font found."));
	}

	settings.fontPath = (*maybeFont)[0].as<std::string>();

	// get the background colour
	const std::string backgroundColourPath =
		(boost::format("$.%1%.%2%.%3%") % kTextboxProperty % kBackgroundColourProperty % kColourProperty).str();

	boost::optional<jsoncons::json> maybeBackgroundColourJson =
		configManager.getSetting(backgroundColourPath);

	if(maybeBackgroundColourJson != boost::none){
		boost::optional<Colour> maybeColour = getColour(*maybeBackgroundColourJson);

		if(maybeColour != boost::none)
			settings.backgroundColour = *maybeColour;
	}	

	return settings;
}

}}
