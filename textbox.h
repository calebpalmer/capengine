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
	virtual bool canFocus() const override;
	virtual bool doFocus(bool, int, int, int, int) override;

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
	
private:
	TextBox(std::string initialText);

	void updateTexture();
	static DisplaySettings getDisplaySettings();
	
	Rect m_rect = {0, 0, 0, 0}; //<! The location of the textboxs bounding box.
	Rect m_boxRect = {0, 0, 0, 0}; //<! The size and location of the actual textbox.
	bool m_hasFocus = false; //<! flag indicating if textbox has focus.
	std::string m_text; //<! The text to display.
	TexturePtr m_texture; //<! The texture to display.
	bool m_textureDirty = true; //<! flag indicating if texture needs to be updated.
	DisplaySettings m_displaySettings; //<! The display settings
	Font m_font;  //<! The font to use for rendering the text.
	
};

}}

#endif // CAPENGING_TEXTBOX_H
