#ifndef CAPENGINE_TEXTBOX_H
#define CAPENGINE_TEXTBOX_H

#include "captypes.h"
#include "commandmanager.h"
#include "font.h"
#include "uiconfigmanager.h"
#include "widget.h"

#include <functional>
#include <map>
#include <optional>

namespace CapEngine
{
namespace UI
{

//! A textbox widget for text input.
class TextBox final : public Widget
{
public:
  static std::shared_ptr<TextBox> create(std::string initialText = "");

  virtual SDL_Rect getPosition() const override;
  virtual void setPosition(int x, int y) override;
  virtual void setSize(int width, int height) override;
  virtual void render() override;
  virtual void update(double ms) override;
  virtual bool canFocus() const override;
  virtual bool doFocus(bool, int, int, int, int) override;

  // handled events
  virtual void handleMouseMotionEvent(SDL_MouseMotionEvent event) override;
  virtual void handleMouseButtonEvent(SDL_MouseButtonEvent event) override;
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

private:
  explicit TextBox(std::string initialText);

  TextBox(const TextBox &) = delete;
  TextBox &operator=(const TextBox &) = delete;

  void updateTexture();
  void unsetSelection();
  void deleteText();
  bool deleteSelectedText();
  void insertText(const std::string &text);

  std::unique_ptr<Command> makeInsertTextCommand(std::string text);
  std::unique_ptr<Command> makeDeleteTextCommand(std::size_t pos,
                                                 std::size_t len);

  int getCursorPositionFromMousePosition(int x, int y) const;
  void setCursorSelectStart(int pos);
  void setCursorSelectEnd(int pos);
  void copySelectionToClipboard(bool deleteSelection = false);

  bool isTextSelected() const;
  std::string getTextBeforeSelection() const;
  std::string getSelectedText() const;
  std::string getTextAfterSelection() const;

  // keypress handlers
  void registerKeypressHandlers();
  void handleBackspaceKey(const SDL_KeyboardEvent &event);
  void handleEscapeKey(const SDL_KeyboardEvent &event);
  void handleDeleteKey(const SDL_KeyboardEvent &event);
  void handleCKey(const SDL_KeyboardEvent &event);
  void handleXKey(const SDL_KeyboardEvent &event);
  void handleVKey(const SDL_KeyboardEvent &event);
  void handleZKey(const SDL_KeyboardEvent &event);
  void handleYKey(const SDL_KeyboardEvent &event);
  void handleAKey(const SDL_KeyboardEvent &event);
  void handleRightArrowKey(const SDL_KeyboardEvent &event);
  void handleLeftArrowKey(const SDL_KeyboardEvent &event);
  void handleEndKey(const SDL_KeyboardEvent &event);
  void handleHomeKey(const SDL_KeyboardEvent &event);

  static DisplaySettings getDisplaySettings();

  Rect m_rect = {0, 0, 0, 0}; //<! The location of the textboxs bounding box.
  Rect m_boxRect = {0, 0, 0,
                    0}; //<! The size and location of the actual textbox.
  Rect m_textRect = {0, 0, 0, 0};
  bool m_hasFocus = false; //<! flag indicating if textbox has focus.
  std::string m_text;      //<! The text to display.
  TexturePtr m_texture;    //<! The texture to display.
  bool m_textureDirty =
      true; //<! flag indicating if texture needs to be updated.
  DisplaySettings m_displaySettings; //<! The display settings
  Font m_font; //<! The font to use for rendering the text.

  // mouse info cursor
  SDL_Cursor *m_pPreviousCursor = nullptr; //<! The previous cursor
  static SDL_Cursor
      *s_pHoverCursor; //<! The cursor to show when hovering over the textbox.
  unsigned int m_cursorTimerMs = 0; //<! used for timing cursor visibility.
  bool m_cursorState = false;       //<! true=visible, not visible otherwise.
  std::optional<std::pair<int, int>>
      m_lastMouseDownPosition; //<! The previous mouse down position.

  int m_cursorPosition = 0;    //<! The position of the cursor
  int m_cursorSelectStart = 0; //<! if selecting text,
  int m_cursorSelectEnd = 0;   //<! if selecting text.

  //! handlers for various keypresses
  std::map<SDL_Keycode, std::function<void(const SDL_KeyboardEvent &)>>
      m_keyPressHandlers;
  //! command manager for undo/redo
  CommandManager m_commandManager;
};

inline bool TextBox::isTextSelected() const
{
  return m_cursorSelectStart != m_cursorSelectEnd;
}

/**
 \fn TextBox::isTextSelected
 \brief check if there is text selected.
 \return
   \li true if there is selected text, false otherwise.
*/

} // namespace UI
} // namespace CapEngine

#endif // CAPENGING_TEXTBOX_H
