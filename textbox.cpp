#include "textbox.h"

#include "collision.h"
#include "uiconfigmanager.h"
#include "logger.h"
#include "locator.h"
#include "colour.h"
#include "scopeguard.h"

#include <boost/format.hpp>
#include <atomic>

namespace CapEngine { namespace UI {

namespace {

const char kTextboxProperty[] = "textbox";
const char kBackgroundColourProperty[] = "background-colour";
const char kFontColourProperty[] = "font-colour";
const char kColourProperty[] = "colour";

const unsigned int kCursorBlinkTime = 500; // ms

//! Checks if there is a special key squence input.
/** 
 \param textInputEvent
   \li The TextInput even to check
 \return 
   \li true if it's a special key sequence, false otherwise
*/
bool isSpecialKeySequence(const SDL_TextInputEvent &textInputEvent){
	if( (textInputEvent.text[0] == 'c' || textInputEvent.text[0] == 'C'  ||
			 textInputEvent.text[0] == 'v' || textInputEvent.text[0] == 'V') &&
			 SDL_GetModState() & KMOD_CTRL )
	{
		return true;
	}

	return false;
}

} // namespace

const Colour TextBox::kDefaultFontColour{0, 0, 0, 255};
const Colour TextBox::kDefaultBackgroundColour{255, 255, 255, 255};

SDL_Cursor *TextBox::s_pHoverCursor = nullptr;

//! Constructor
/** 
 \param initialText
   \li any initial text to put in the textbox.
*/
TextBox::TextBox(std::string initialText)
	: m_text(std::move(initialText)), m_texture(getNullTexturePtr()),
		m_displaySettings(getDisplaySettings()),
		m_font(Font(m_displaySettings.fontPath, m_displaySettings.fontSize)),
		m_cursorPosition(m_text.size())
{
	static std::atomic<bool> cursorInitialized = false;
	if(!std::atomic_exchange(&cursorInitialized, true)){
		s_pHoverCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	}
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

//! \copydoc Widget::update
void TextBox::update(double ms){
	//update cursor state
	if(m_cursorTimerMs + ms >= kCursorBlinkTime){
		m_cursorState = !m_cursorState;
		m_cursorTimerMs = (m_cursorTimerMs + static_cast<unsigned int>(ms + 0.5)) % kCursorBlinkTime;
	}
	else{
		m_cursorTimerMs += ms;
	}
}

//! \copydoc Widget::render
void TextBox::render() {
	if(m_textureDirty)
		this->updateTexture();

	// draw the box
	assert(Locator::videoManager != nullptr);
	Locator::videoManager->drawFillRect(m_windowId, m_boxRect, m_displaySettings.backgroundColour);

	assert(m_texture != nullptr);
	int textureWidth = Locator::videoManager->getTextureWidth(m_texture.get());

	if(m_textRect.x == 0 && m_textRect.y == 0 &&
		 m_textRect.w == 0 && m_textRect.h == 0)
	{
		m_textRect.x = m_boxRect.x;
		m_textRect.y = m_boxRect.y;
		m_textRect.h = m_boxRect.h;
	}
	
	if(m_textRect.w != textureWidth && textureWidth > m_boxRect.w){

		m_textRect.w = textureWidth;
		// move it to the end, and then adjust for the cursor?
		int difference = m_boxRect.w - textureWidth;
		m_textRect.x = m_boxRect.x - difference;
	}

	// find where the cursor would be drawn if we have focus
	int verticalPadding = 2;

	int cursorDrawOffset = 0;
	// draw the cursor
	if(m_cursorPosition > 0){
		//int width = Locator::videoManager->getTextureWidth(m_texture.get());
		int width = m_font.getTextSize(m_text.substr(0, m_cursorPosition));
		cursorDrawOffset = width;
	}
		
	// draw the text
	if(m_texture != nullptr){
		ScopeGuard guard([&]() { Locator::videoManager->setClipRect(m_windowId, nullptr); });	
		Locator::videoManager->setClipRect(m_windowId, &m_boxRect);

		// cursor position is not visible to the left
		while(m_textRect.x + cursorDrawOffset < m_boxRect.x){
			m_textRect.x++;
		}

		// cursor position is not visible to the right
		while(m_textRect.x + cursorDrawOffset > m_boxRect.x + m_boxRect.w){
			m_textRect.x--;
		}

		Locator::videoManager->drawTexture(m_windowId, m_textRect.x, m_textRect.y, m_texture.get());

	}

	// draw the cursor
	if(m_hasFocus && m_cursorState == true){
		Locator::videoManager->drawLine(m_windowId,
																		m_textRect.x + cursorDrawOffset,
																		m_boxRect.y + verticalPadding,
																		m_textRect.x + cursorDrawOffset,
																		m_boxRect.y + m_boxRect.h - verticalPadding,
																		{0, 0, 0, 255});
	}
	
}

//! \copydoc Widget::canFocus
bool TextBox::canFocus() const {
	return true;
}

//! \copydoc Widget::doFocus
bool TextBox::doFocus(bool focus, int downX, int downY, int upX, int upY) {
	if(focus == false) {
		if(m_hasFocus){
			SDL_StopTextInput();
			assert(!SDL_IsTextInputActive());
			
			m_hasFocus = false;
		}

		return false;
	}

	else {
		if(pointInRect( { downX, downY }, m_boxRect) && pointInRect( { upX, upY }, m_boxRect)){
			if(!m_hasFocus){

				SDL_StartTextInput();
				SDL_SetTextInputRect(nullptr);
				
				assert(SDL_IsTextInputActive());
				
				m_hasFocus = true;
				m_cursorTimerMs = 0;
				m_cursorState = false;
			}
			
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
	if(m_text.size() > 0){
		Colour textColour{0, 0, 0, 255};
		SurfacePtr surfacePtr = m_font.getTextSurface(m_text, textColour);

		assert(Locator::videoManager != nullptr);
		m_texture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, surfacePtr.get());
	}
	else{
		m_texture.release();
	}
	
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

//! \copydoc Widget::handleMouseMotionEvent
void TextBox::handleMouseMotionEvent(SDL_MouseMotionEvent event){
	assert(s_pHoverCursor != nullptr);
	SDL_Cursor *pCurrentCursor = SDL_GetCursor();

	// no mouse?
	if(pCurrentCursor == nullptr)
		return;
	
	if(pointInRect({event.x, event.y}, m_boxRect) &&
		  pCurrentCursor != s_pHoverCursor)
	{
		m_pPreviousCursor = pCurrentCursor;
		SDL_SetCursor(s_pHoverCursor);
	}

	else{
		if(pCurrentCursor == s_pHoverCursor){
			SDL_SetCursor(m_pPreviousCursor);
		}
	}
}

// \copydoc Widget::handleKeyboardEvent
void TextBox::handleKeyboardEvent(SDL_KeyboardEvent event){
	if(m_hasFocus){
		SDL_Keycode keycode = event.keysym.sym;
		
		// keydown
		if(event.type == SDL_KEYDOWN){

			// backspace
			if(keycode == SDLK_BACKSPACE && m_text.size() > 0 && m_cursorPosition > 0){
				m_text.erase(m_cursorPosition - 1, 1);

				m_cursorPosition--;
				if(m_cursorPosition < 0)
					m_cursorPosition = 0;

				m_textureDirty = true;
			}

			// ctrl + c = copy
			else if(keycode == SDLK_c && SDL_GetModState() & KMOD_CTRL){
				//SDL_SetClipboardText( inputText.c_str() );
				// Not yet supported
			}

			// ctrl + v = paste
			else if(keycode == SDLK_v && SDL_GetModState() & KMOD_CTRL ){
				m_text += SDL_GetClipboardText();
			}
			
			// shift key, start or stop text selection
			else if(keycode == SDLK_LSHIFT || keycode == SDLK_RSHIFT){
					m_cursorSelectStart = m_cursorPosition;
					m_selectionState = SelectionState::KeyboardSelection;
			}

			// escape
			else if(keycode == SDLK_ESCAPE){
				m_cursorSelectStart = std::nullopt;
			}

			// move cursor to the right
			else if(keycode == SDLK_RIGHT){
				m_cursorPosition++;;
				if(m_cursorPosition > static_cast<int>(m_text.size()))
					m_cursorPosition = m_text.size();

			}

			// move cursor to the left
			else if(keycode == SDLK_LEFT){
				m_cursorPosition--;
				if(m_cursorPosition < 0)
					m_cursorPosition = 0;
			}

		}
		// keyup
		else{
			if(keycode == SDLK_LSHIFT || keycode == SDLK_RSHIFT){
				m_selectionState = SelectionState::NoSelection;
			}
		}
	}
}

//! \copydoc Widget::handleTextInputEvent
void TextBox::handleTextInputEvent(SDL_TextInputEvent event){
	if(!(isSpecialKeySequence(event)) && m_hasFocus)
	{
		m_text += event.text;
		m_cursorPosition++;
		m_textureDirty = true;
	}
}

}}
