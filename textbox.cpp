#include "textbox.h"

#include "collision.h"
#include "uiconfigmanager.h"
#include "logger.h"
#include "locator.h"
#include "colour.h"
#include "scopeguard.h"
#include "simplecommand.h"

#include <boost/format.hpp>
#include <atomic>
#include <functional>

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
		m_cursorPosition(m_text.size()), m_cursorSelectStart(m_cursorPosition),
		m_cursorSelectEnd(m_cursorPosition)
{
	static std::atomic<bool> cursorInitialized = false;
	if(!std::atomic_exchange(&cursorInitialized, true)){
		s_pHoverCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	}

	registerKeypressHandlers();
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

	int textureWidth = m_texture == nullptr ? 0 : Locator::videoManager->getTextureWidth(m_texture.get());

	if(m_textRect.x == 0 && m_textRect.y == 0 &&
		 m_textRect.w == 0 && m_textRect.h == 0)
	{
		m_textRect.x = m_boxRect.x;
		m_textRect.y = m_boxRect.y;
		m_textRect.h = m_boxRect.h;
		m_textRect.w = textureWidth;
	}
	
	if(m_textRect.w != textureWidth && textureWidth > m_boxRect.w){

		m_textRect.w = textureWidth;
		// move it to the end, and then adjust for the cursor?
		int difference = m_boxRect.w - textureWidth;
		m_textRect.x = m_boxRect.x - difference;
	}

	// draw the blinking cursor
	// find where the cursor would be drawn if we have focus
	int verticalPadding = 2;

	int cursorDrawOffset = 0;
	if(m_cursorPosition > 0){
		assert(m_cursorPosition <= static_cast<int>(m_text.size()));
		int width = m_font.getTextSize(m_text.substr(0, m_cursorPosition));
		cursorDrawOffset = width;
	}

	if(m_hasFocus && m_cursorState == true){
		Locator::videoManager->drawLine(m_windowId,
																		m_textRect.x + cursorDrawOffset,
																		m_boxRect.y + verticalPadding,
																		m_textRect.x + cursorDrawOffset,
																		m_boxRect.y + m_boxRect.h - verticalPadding,
																		{0, 0, 0, 255});
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

//! Update the texture that is displayed.
void TextBox::updateTexture() {
	// get the height of the textbox based on the font
	m_boxRect.w = m_rect.w;
	m_boxRect.h = m_font.getFontHeight();
	m_boxRect.x = m_rect.x;
	m_boxRect.y = (m_rect.y + (m_rect.h / 2)) - (m_boxRect.h / 2);

	// render the text
	if(m_text.size() > 0){
		assert(Locator::videoManager != nullptr);

		Colour textColour{0, 0, 0, 255};

		// there is not text selected
		if(m_cursorSelectStart == m_cursorSelectEnd){
			SurfacePtr surfacePtr = m_font.getTextSurface(m_text, textColour);
			assert(surfacePtr != nullptr);

			m_texture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, surfacePtr.get());
		}

		// text is selected
		else {
			assert(m_cursorSelectStart < m_cursorSelectEnd);
			
			SurfacePtr beforeText = getNullSurfacePtr();
			SurfacePtr selectedText = getNullSurfacePtr();
			SurfacePtr afterText = getNullSurfacePtr();

			// render non selected at beginning if there is any
			if(m_cursorSelectStart > 0){
				beforeText = m_font.getTextSurface(this->getTextBeforeSelection(), textColour);
			}
			
			// render the selected text
			Colour selectedTextColour{255, 255, 255, 255};
			Colour bgColour{100,100,100,255};
			selectedText = m_font.getTextSurface(this->getSelectedText(), selectedTextColour, bgColour);
			assert(selectedText != nullptr);
			

			// render non selected text at the end if there is any
			if(m_cursorSelectEnd < static_cast<int>(m_text.size())){
				afterText = m_font.getTextSurface(this->getTextAfterSelection(), textColour);
			}

			// stitch them together
			int textureWidth = Locator::videoManager->getSurfaceWidth(selectedText.get()) +
				(beforeText != nullptr ? Locator::videoManager->getSurfaceWidth(beforeText.get()) : 0) +
				(afterText != nullptr ? Locator::videoManager->getSurfaceWidth(afterText.get()) : 0);

			int textureHeight = m_boxRect.h;
			SurfacePtr combinedText = Locator::videoManager->createSurfacePtr(textureWidth, textureHeight);

			// there is text before selected text
			if(beforeText != nullptr){
				int beforeTextWidth = Locator::videoManager->getSurfaceWidth(beforeText.get());
				Locator::videoManager->blitSurface(beforeText.get(), 0, 0,
																					 beforeTextWidth,
																					 Locator::videoManager->getSurfaceHeight(beforeText.get()),
																					 combinedText.get(), 0, 0);

				Locator::videoManager->blitSurface(selectedText.get(), 0, 0,
																					 Locator::videoManager->getSurfaceWidth(selectedText.get()),
																					 Locator::videoManager->getSurfaceHeight(selectedText.get()),
																					 combinedText.get(), beforeTextWidth, 0);
			}

			// no text before selected text
			else{
				Locator::videoManager->blitSurface(selectedText.get(), 0, 0,
																					 Locator::videoManager->getSurfaceWidth(selectedText.get()),
																					 Locator::videoManager->getSurfaceHeight(selectedText.get()),
																					 combinedText.get(), 0, 0);
				
			}

			// there is text after selected text
			if(afterText != nullptr){
				int xStart = beforeText != nullptr ?
					Locator::videoManager->getSurfaceWidth(beforeText.get()) + Locator::videoManager->getSurfaceWidth(selectedText.get()) :
					Locator::videoManager->getSurfaceWidth(selectedText.get());
					
				Locator::videoManager->blitSurface(afterText.get(), 0, 0,
																					 Locator::videoManager->getSurfaceWidth(afterText.get()),
																					 Locator::videoManager->getSurfaceHeight(afterText.get()),
																					 combinedText.get(),
																					 xStart, 0);

			}

			
			m_texture = Locator::videoManager->createTextureFromSurfacePtr(m_windowId, combinedText.get());
		}
	}
	else{
		// no text, no need to hold onto a texture anymore.
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

	// check if cursor needs to be changed if in the textbox
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

	// do possible text selection
	if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		int prevCursorPosition =
			this->getCursorPositionFromMousePosition(m_lastMouseDownPosition->first, m_lastMouseDownPosition->second);
		
		int currentCursorPosition =
			this->getCursorPositionFromMousePosition(event.x, event.y);

		// check to see if dragging was done for text selection
		if(currentCursorPosition != prevCursorPosition){
			if(prevCursorPosition < currentCursorPosition){
				this->setCursorSelectStart(prevCursorPosition);
				this->setCursorSelectEnd(currentCursorPosition);
			}

			else{
				this->setCursorSelectEnd(prevCursorPosition);
				this->setCursorSelectStart(currentCursorPosition);
			}
		}
	}
}

//! \copydoc Widget::handleMouseButtonEvent
void TextBox::handleMouseButtonEvent(SDL_MouseButtonEvent event){
	if(event.button == SDL_BUTTON_LEFT && event.state == SDL_PRESSED){
		m_lastMouseDownPosition = std::make_pair(event.x, event.y);
	}
	
	else if(event.button == SDL_BUTTON_LEFT && event.state == SDL_RELEASED){

		// only set the cursor based on click if the button down and button released
		// were in the textbox.
		if(m_lastMouseDownPosition != std::nullopt &&
			 pointInRect(Point(*m_lastMouseDownPosition), m_boxRect) &&
			 pointInRect(Point{event.x, event.y}, m_boxRect))
		{
			int currentCursorPosition =
				this->getCursorPositionFromMousePosition(event.x, event.y);

			// set the current cursor position
			m_cursorPosition = currentCursorPosition;
			m_lastMouseDownPosition = std::nullopt;
		}
	}
}

// \copydoc Widget::handleKeyboardEvent
void TextBox::handleKeyboardEvent(SDL_KeyboardEvent event){
	if(m_hasFocus){
		SDL_Keycode keycode = event.keysym.sym;

		auto handler = m_keyPressHandlers.find(keycode);
		if(handler != m_keyPressHandlers.end()){
			handler->second(event);
		}
	}
}

//! \copydoc Widget::handleTextInputEvent
void TextBox::handleTextInputEvent(SDL_TextInputEvent event){
	if(!(isSpecialKeySequence(event)) && m_hasFocus)
	{
		if(this->isTextSelected()){
			this->deleteSelectedText();
		}
		
		this->insertText(event.text);
		m_textureDirty = true;
	}
}


//! unselects any selected text.
void TextBox::unsetSelection(){
	this->setCursorSelectStart(m_cursorPosition);
	this->setCursorSelectEnd(m_cursorPosition);
}

//! deletes a selection if there is one
void TextBox::deleteText(){
	if(!this->isTextSelected()){
		if(m_text.size() > static_cast<size_t>(m_cursorPosition)){
			m_commandManager.runCommand(this->makeDeleteTextCommand(m_cursorPosition, 1));
		}
	}
	else{
		this->deleteSelectedText();
	}
}


//! Deletes selected text if there is any.
/** 
 \return 
   \li true if text was deleted, false otherwise.
*/
bool TextBox::deleteSelectedText(){
	if(this->isTextSelected()){
		auto deleteCommand = this->makeDeleteTextCommand(m_cursorSelectStart, m_cursorSelectEnd - m_cursorSelectStart);
		m_commandManager.runCommand(std::move(deleteCommand));

		return true;
	}

	return false;
}


//! insert text at the current cursor location.
/** 
 \param text
   \li The text to insert.
*/
void TextBox::insertText(const std::string &text){
	m_commandManager.runCommand(this->makeInsertTextCommand(text));
}


//! Makes a Command object for inserting text.
/** 
 \param text
   \li The text to insert.
  \return
	  \li The Command
*/
std::unique_ptr<Command> TextBox::makeInsertTextCommand(std::string in_text) {
	auto executeCommand = [this, text = in_text]()
												{
													assert(m_cursorPosition <= static_cast<int>(m_text.size()));
													m_text.insert(m_cursorPosition, text);
													m_cursorPosition += text.size();
													unsetSelection();
													m_textureDirty = true;
												};
	auto unExecuteCommand = [this, currentCursorPosition = this->m_cursorPosition, text = std::move(in_text)]()
													{
														assert(currentCursorPosition + text.size() <= m_text.size());
														m_text.erase(currentCursorPosition, text.size());
														m_cursorPosition = currentCursorPosition;
														this->unsetSelection();
														m_textureDirty = true;
													};

	return std::make_unique<SimpleCommand>(executeCommand, unExecuteCommand);
}

//! Makes a Command object for deleting text.
/** 
 \param pos
   \li The position to start deletion from.
 \param
   \li The number of characters to delete.
  \return
	  \li The Command
*/
std::unique_ptr<Command> TextBox::makeDeleteTextCommand(std::size_t pos, std::size_t len) {
	auto executeCommand = [this, pos, len]()
												{
													assert(pos + len <= m_text.size());
													m_text.erase(pos, len);
													m_cursorPosition = pos;
													unsetSelection();
													m_textureDirty = true;
												};
	auto unExecuteCommand = [this, pos, text = m_text.substr(pos, len)]()
													{
														assert(pos <= m_text.size());
														m_text.insert(pos, text);
														m_cursorPosition = pos + text.size();
														this->unsetSelection();
														m_textureDirty = true;
													};

	return std::make_unique<SimpleCommand>(executeCommand, unExecuteCommand);
}


//! Gets text before selection
/** 
 \return 
   \li The text.
*/
std::string TextBox::getTextBeforeSelection() const {
	assert(m_cursorSelectStart <= static_cast<int>(m_text.size()));
	
	if(m_cursorSelectStart == 0){
		return "";
	}

	return m_text.substr(0, m_cursorSelectStart);
}

//! Gets text after selection
/** 
 \return 
   \li The text.
*/
std::string TextBox::getTextAfterSelection() const {
	if(m_cursorSelectStart != m_cursorSelectEnd &&
		 m_cursorSelectEnd < static_cast<int>(m_text.size()))
	{
		return m_text.substr(m_cursorSelectEnd, static_cast<int>(m_text.size()) - m_cursorSelectEnd);
	}

	return "";
}

//! Gets selected text.
/** 
 \return 
   \li The text.
*/
std::string TextBox::getSelectedText() const {
	if(m_cursorSelectStart != m_cursorSelectEnd){
		assert(m_cursorSelectStart < m_cursorSelectEnd);
		assert(m_cursorSelectStart >= 0);
		assert(m_cursorSelectEnd <= static_cast<int>(m_text.size()));
		
		return m_text.substr(m_cursorSelectStart, m_cursorSelectEnd - m_cursorSelectStart);
	}

	return "";
}

//! Registers the keypress handler functions
void TextBox::registerKeypressHandlers(){
	m_keyPressHandlers.emplace(SDLK_BACKSPACE, std::bind(&TextBox::handleBackspaceKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_ESCAPE, std::bind(&TextBox::handleEscapeKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_DELETE, std::bind(&TextBox::handleDeleteKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_c, std::bind(&TextBox::handleCKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_x, std::bind(&TextBox::handleXKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_v, std::bind(&TextBox::handleVKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_z, std::bind(&TextBox::handleZKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_y, std::bind(&TextBox::handleYKey, this, std::placeholders::_1));	
	m_keyPressHandlers.emplace(SDLK_a, std::bind(&TextBox::handleAKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_RIGHT, std::bind(&TextBox::handleRightArrowKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_LEFT, std::bind(&TextBox::handleLeftArrowKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_HOME, std::bind(&TextBox::handleHomeKey, this, std::placeholders::_1));
	m_keyPressHandlers.emplace(SDLK_END, std::bind(&TextBox::handleEndKey, this, std::placeholders::_1));
}


//! handler for backspace key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleBackspaceKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN){
		if(m_cursorSelectStart != m_cursorSelectEnd){
			this->deleteText();
		}
		else{
			if(m_cursorPosition > 0){
				m_commandManager.runCommand(this->makeDeleteTextCommand(m_cursorPosition - 1, 1));
			}
		}
	}
}

//! handler for escape key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleEscapeKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{
		this->unsetSelection();
	}
}

//! handler for delete key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleDeleteKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{	
		this->deleteText();
	}
}

//! Copies the selected text, if any.  Optionally delete the selected text.
/**
	 \param deleteSelection
	   \li if true, the selected text is also deleted.
 */
void TextBox::copySelectionToClipboard(bool deleteSelection){
	if(this->isTextSelected()){
		SDL_SetClipboardText(this->getSelectedText().c_str());

		if(deleteSelection){
			this->deleteSelectedText();
		}
	}

	else{
		SDL_SetClipboardText(m_text.c_str());
	}
}

//! handler for C key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleCKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{		
		if(SDL_GetModState() & KMOD_CTRL){
			this->copySelectionToClipboard();
		}
	}
}

//! handle for X key
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleXKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{		
		if(SDL_GetModState() & KMOD_CTRL){
			this->copySelectionToClipboard(true);
		}
	}
}

//! handle for Z key
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleZKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{		
		if(SDL_GetModState() & KMOD_CTRL){
			m_commandManager.undo();
		}
	}
}

//! handle for Y key
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleYKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{		
		if(SDL_GetModState() & KMOD_CTRL){
			m_commandManager.redo();
		}
	}
}

//! handler for V key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleVKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{	
		if(SDL_GetModState() & KMOD_CTRL){
			if(this->isTextSelected()){
				this->deleteSelectedText();
			}

			this->insertText(SDL_GetClipboardText());
		}
	}
}

//! handler for A key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleAKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN)	{	
		if(SDL_GetModState() & KMOD_CTRL){
			this->setCursorSelectStart(0);
			this->setCursorSelectEnd(m_text.size());
		}
	}
}

//! handler for backspacright arrow key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleRightArrowKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN){
		// update the cursor position
		m_cursorPosition++;
		bool moved = true;
		if(m_cursorPosition > static_cast<int>(m_text.size())){
			m_cursorPosition = m_text.size();
			moved = false;
		}

		if(moved)
			m_textureDirty = true;

		// set selection end position if selecting
		if(SDL_GetModState() & KMOD_SHIFT && moved){
			if(m_cursorSelectEnd >= m_cursorPosition &&
				 m_cursorSelectEnd >= m_cursorSelectStart)
				this->setCursorSelectStart(m_cursorSelectStart + 1);

			else
				this->setCursorSelectEnd(m_cursorSelectEnd + 1);

		}
		else{
			this->unsetSelection();
		}
	}
}

//! handler for left arrow key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleLeftArrowKey(const SDL_KeyboardEvent &event){
	if(event.type == SDL_KEYDOWN){	
		// update the cursor position
		m_cursorPosition--;

		bool moved = true;
		if(m_cursorPosition < 0){
			m_cursorPosition = 0;
			moved = false;
		}
				
		if(moved)
			m_textureDirty = true;
				
		// set selection end position if selecting
		if(SDL_GetModState() & KMOD_SHIFT && moved){
			if(m_cursorSelectStart <= m_cursorPosition  &&
				 m_cursorSelectStart < m_cursorSelectEnd)
				this->setCursorSelectEnd(m_cursorSelectEnd- 1);

			else
				this->setCursorSelectStart(m_cursorSelectStart - 1);

		}
		else{
			this->unsetSelection();
		}
	}
}

//! handler for home key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleHomeKey(const SDL_KeyboardEvent &event){
	if(SDL_GetModState() & KMOD_SHIFT){
		this->setCursorSelectStart(0);
		m_cursorPosition = 0;
		m_textureDirty = true;
	}

	else{
		m_cursorPosition = 0;
		this->unsetSelection();

	}
}

//! handler for end key.
/** 
 \param event
   \li The keyboard event.
*/
void TextBox::handleEndKey(const SDL_KeyboardEvent& ){
	auto position = m_text.size();

	if(SDL_GetModState() & KMOD_SHIFT){
		m_cursorSelectEnd = position;
		m_cursorPosition = position;
		m_textureDirty = true;
	}

	else{
		m_cursorPosition = position;
		this->unsetSelection();

	}
}


//! gets the cursor position based on mouse click position.
/** 
 \param x
   \li The x position of the mouse.
 \param y
   \li y The y position of the mouse.
 \return 
   \li The cursor position.
*/
int TextBox::getCursorPositionFromMousePosition(int x, int y) const {
	int cursorPosition = 0;

	if(x > m_textRect.x + m_textRect.w){
		cursorPosition = m_text.size();
	}

	else if(x < m_textRect.x){
		cursorPosition = 0;
	}

	else{

		// TODO a better algorithm here would be nice
		for(unsigned int i = 1; i < m_text.size(); i++){
			auto textWidth = m_font.getTextSize(m_text.substr(0, i));
			if(x <= m_textRect.x + textWidth){
				cursorPosition = i;
				break;
			}
		}
	}

	return cursorPosition;
}


//! Sets the cursor selection start position.
/** 
 \param pos
   \li The start position of selected text.
*/
void TextBox::setCursorSelectStart(int pos){
	if(m_cursorSelectStart != pos){
		m_textureDirty = true;
	}

	m_cursorSelectStart = pos;
}

//! Sets the cursor selection end position.
/** 
 \param pos
   \li The end position of selected text.
*/
void TextBox::setCursorSelectEnd(int pos){
	if(m_cursorSelectEnd != pos){
		m_textureDirty = true;
	}

	m_cursorSelectEnd = pos;
}

}}
