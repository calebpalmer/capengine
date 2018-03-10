#include "uibutton.h"

#include "label.h"
#include "locator.h"
#include "uiconfigmanager.h"
#include "utils.h"

#include <jsoncons_ext/jsonpath/json_query.hpp>

namespace CapEngine { namespace UI {

namespace {

const int kMaxHeight = 48;

const Colour kDefaultNeutralFontColour = {0, 0, 0, 255};
const Colour kDefaultNeutralBackgroundColour = {43, 140, 229, 255};
const Colour kDefaultHoverFontColour = {0, 0, 0, 255};
const Colour kDefaultHoverBackgroundColour = { 43,  140, 229, 255};
const Colour kDefaultPressedFontColour = {0, 0, 0, 255};
const Colour kDefaultPressedBackgroundColour = { 43,  57, 229, 255};

}

//! Constructor
/** 
 \param text - The text to display.
*/
Button::Button(const std::string &text)
	: m_text(text)
{
	m_pLabel = Label::create(m_text);
	readUIConfig();
}

//! Creates a new Button
/** 
 \param text - The text to display.
 \return - The Button.
*/
std::shared_ptr<Button> Button::create(const std::string &text){
	return std::shared_ptr<Button>(new Button(text));
}


//! \copydoc Widget::setWindowId
void Button::setWindowId(Uint32 windowId){
	m_windowId = windowId;

	assert(m_pLabel);
	m_pLabel->setWindowId(windowId);
}


//! \copydoc Widget::setPosition
void Button::setPosition(int x, int y){
	m_rect.x = x;
	m_rect.y = y;

	m_updateLabel = true;
}

//! \copydoc Widget::setSize
void Button::setSize(int width, int height){
	m_rect.w = width;
	m_rect.h = height;

	m_updateLabel = true;
}

//! \copydoc Widget::render
void Button::render(){
	// draw the button
	assert(Locator::videoManager != nullptr);

	SDL_Rect dstRect = this->getRenderRect();
	if(m_state == State::Neutral){
		Locator::videoManager->drawFillRect(m_windowId, dstRect, m_uiConfig.neutralBackgroundColour);
	}
	
	// draw the label
	assert(m_pLabel != nullptr);
	if(m_updateLabel)
		this->updateChildren();
	
	m_pLabel->render();
}


//! Get the text of the button.
/** 
 \return - The text of the button.
*/
std::string Button::getText() const{
	return m_text;
}



//! Updates the text of the button
/** 
 \param text - The text to set on the button.
*/
void Button::setText(const std::string & text){
	m_text = text;
	m_pLabel = Label::create(m_text);	
}

//! updates the size and position of the child label
void Button::updateChildren(){
	SDL_Rect containingRect = this->getRenderRect();
	
	assert(m_pLabel);
 	m_pLabel->setPosition(containingRect.x, containingRect.y);
 	m_pLabel->setSize(containingRect.w, containingRect.h);	
}

//! Reads the ui config into m_uiConfig
void Button::readUIConfig(){
 	UIConfigManager &configMan = UIConfigManager::getInstance();

 	const std::string buttonPath = "$.button";
 	boost::optional<jsoncons::json> maybeSettings = configMan.getSetting(buttonPath);
	if(!maybeSettings)
		CAP_THROW(CapEngineException("Could not load button settings. jsonpath query: " + buttonPath));
	jsoncons::json settings = (*maybeSettings)[0];

	// get the font path
	jsoncons::json fontpath = jsoncons::jsonpath::json_query(settings, "$.font.path");
	if(fontpath.empty()){
		boost::optional<jsoncons::json> defaultFontPath = configMan.getSetting(kDefaultFontSettingsPath);
		if(!defaultFontPath)
			CAP_THROW(CapEngineException("Could not get default font."))

		m_uiConfig.fontpath = (*defaultFontPath)[0].as<std::string>();
	}

	// neutral settings
	if(!settings.has_key("neutral"))
		 CAP_THROW(CapEngineException("Button settings missing \"neutral\" property."));


	auto getColourSetting = [](const jsoncons::json colourParent) -> boost::optional<Colour> {
		if(!colourParent.has_key("colour"))
			return boost::none;
		else{
			boost::optional<Colour> maybeColour = getColour(colourParent["colour"]);
			if(maybeColour)
				return *maybeColour;
			else
				return boost::none;
		}
	};

	// neutral settings
	jsoncons::json neutralSettings = settings["neutral"];
	boost::optional<Colour> maybeNeutralFontColour = getColourSetting(getProperty(neutralSettings, "font"));
	m_uiConfig.neutralFontColour = maybeNeutralFontColour ? *maybeNeutralFontColour : kDefaultNeutralFontColour;

	boost::optional<Colour> maybeNeutralBackgroundColour = getColourSetting(getProperty(neutralSettings,"fill"));
	m_uiConfig.neutralBackgroundColour = maybeNeutralBackgroundColour ? *maybeNeutralBackgroundColour : kDefaultNeutralBackgroundColour;

	// hover settings
 	jsoncons::json hoverSettings = getProperty(settings, "hover");
	boost::optional<Colour> maybeHoverFontColour = getColourSetting(getProperty(hoverSettings, "font"));
	m_uiConfig.hoverFontColour = maybeHoverFontColour ? *maybeHoverFontColour : kDefaultHoverFontColour;

	boost::optional<Colour> maybeHoverBackgroundColour = getColourSetting(getProperty(hoverSettings, "fill"));
	m_uiConfig.hoverBackgroundColour = maybeHoverBackgroundColour ? *maybeHoverBackgroundColour : kDefaultHoverBackgroundColour;

	// pressed settings
 	jsoncons::json pressedSettings = getProperty(settings, "pressed");
	boost::optional<Colour> maybePressedFontColour = getColourSetting(getProperty(pressedSettings, "font"));
	m_uiConfig.pressedFontColour = maybePressedFontColour ? *maybePressedFontColour : kDefaultPressedFontColour;

	boost::optional<Colour> maybePressedBackgroundColour = getColourSetting(getProperty(pressedSettings, "fill"));
	m_uiConfig.pressedBackgroundColour = maybePressedBackgroundColour ? *maybePressedBackgroundColour : kDefaultPressedBackgroundColour;
}

//! Gets the Rectangle where the button is rendered.
/** 
 \return The Rectangle.
*/
SDL_Rect Button::getRenderRect() const {
	SDL_Rect dstRect = m_rect;
	if(m_rect.h > kMaxHeight)
		dstRect.h = kMaxHeight;

	return expandRectToFit(dstRect, m_rect);
}

}}
