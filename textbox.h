#ifndef CAPENGINE_TEXTBOX_H
#define CAPENGINE_TEXTBOX_H

#include <optional>

#include "widget.h"
#include "font.h"
#include "captypes.h"
#include "uiconfigmanager.h"

namespace CapEngine { namespace UI {

//! A textbox widget for text input.
class TextBox : public Widget {
public:
	static std::shared_ptr<TextBox> create(std::string initialText="");
	
	virtual SDL_Rect getPosition() const override;
	virtual void setPosition(int x, int y) override;
	virtual void setSize(int width, int height) override;
	virtual void render() override;
	virtual void update(double ms) override;
	virtual bool canFocus() const override;
	virtual bool doFocus(bool, int, int, int, int) override;

	// handled events
	virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
	virtual void handleKeyboardEvent(SDL_KeyboardEvent event) override;
	virtual void handleTextInputEvent(SDL_TextInputEvent event) override;

private:
	static const Colour kDefaultFontColour;
	static const Colour kDefaultBackgroundColour;
	
	//! To store display settings to be used.
	struct DisplaySettings {
		Colour backgroundColour = kDefaultBackgroundColour;
		Colour fontColour = kDefaultFontColour;
		int fontSize = kDefaultFontSize;
		std::string fontPath;
	};

	//! Describes the current state of selection.
	enum class SelectionState {
    NoSelection,
	  MouseSelection,
	  KeyboardSelection
	};
	
private:
	TextBox(std::string initialText);

	void updateTexture();
	static DisplaySettings getDisplaySettings();
	
	Rect m_rect = {0, 0, 0, 0}; //<! The location of the textboxs bounding box.
	Rect m_boxRect = {0, 0, 0, 0}; //<! The size and location of the actual textbox.
	Rect m_textRect = {0, 0, 0, 0};
	bool m_hasFocus = false; //<! flag indicating if textbox has focus.
	std::string m_text; //<! The text to display.
	int m_position = 0; //<! The position in the textbox
	TexturePtr m_texture; //<! The texture to display.
	bool m_textureDirty = true; //<! flag indicating if texture needs to be updated.
	DisplaySettings m_displaySettings; //<! The display settings
	Font m_font;  //<! The font to use for rendering the text.

	SDL_Cursor *m_pPreviousCursor = nullptr; //<! The previous cursor
	static SDL_Cursor *s_pHoverCursor; //<! The cursor to show when hovering over the textbox.
	unsigned int m_cursorTimerMs = 0; //<! used for timing cursor visibility.
	bool m_cursorState = false; //<! true=visible, not visible otherwise.

	//! The current state of selection.
	SelectionState m_selectionState = SelectionState::NoSelection;
	int m_cursorPosition = 0; //<! The position of the cursor
	int m_cursorSelectStart = 0; //<! if selecting text,
	int m_cursorSelectEnd = 0; //<! if selecting text.
	
};

}}

#endif // CAPENGING_TEXTBOX_H
