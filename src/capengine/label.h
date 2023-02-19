#ifndef LABEL_H
#define LABEL_H

#include "captypes.h"
#include "fontmanager.h"
#include "orientable.h"
#include "widget.h"

#include <bits/stdint-uintn.h>
#include <memory>
#include <string>

namespace CapEngine
{
namespace UI
{

class Label : public Widget, public Orientable
{
  public:
    static std::shared_ptr<Label>
        create(const std::string &text,
               std::optional<std::string> font = std::nullopt,
               std::optional<int> fontSize = std::nullopt,
               std::optional<Colour> colour = std::nullopt);

    // Widget virtuals
    virtual SDL_Rect getPosition() const override;
    virtual void setPosition(int x, int y) override;
    virtual void setSize(int width, int height) override;
    virtual void render() override;

    std::string getText() const;
    void setText(const std::string &text);
    std::pair<int, int> getTextureSize();

  private:
    Label(const std::string &text,
          std::optional<std::string> font = std::nullopt,
          std::optional<int> fontSize = std::nullopt,
          std::optional<Colour> colour = std::nullopt);

    void loadTexture();

    int m_x = 0;      //<! The x location of the label (upper left)
    int m_y = 0;      //<! The y location of the label (upper left)
    int m_width = 0;  //<! The width of the label
    int m_height = 0; //<! The height of the label

    std::string m_text = ""; //<! The text to display
    std::string m_font = ""; //<! the font of the label
    int m_fontSize;          //<! The size of the font
    Colour m_colour;         //<! The colour

    TexturePtr m_texture;      //<! The surface
    FontManager m_fontManager; //<! fontmanager for getting text surfaces
};

} // namespace UI
} // namespace CapEngine

#endif // LABEL_H
