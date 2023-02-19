#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include "VideoManager.h"
#include "button.h"
#include "fontmanager.h"
#include "vector.h"

#include <string>

namespace CapEngine
{

class BasicButton : public Button
{
private:
  BasicButton(const BasicButton &);
  BasicButton &operator=(const BasicButton &);

public:
  BasicButton(std::string text, int fontSize, CapEngine::Vector position,
              int width, int height);
  ~BasicButton();

  virtual void update() override;
  virtual void render() override;
  virtual void setEnabled(bool enabled = true) override;
  virtual bool isEnabled() const override;
  virtual int getWidth() const override;
  virtual int getHeight() const override;
  virtual void setWidth(int width) override;
  virtual void setHeight(int height) override;
  virtual std::string getText() const override;
  virtual void setText(const std::string text) override;

protected:
  std::string m_text;
  int m_width;
  int m_height;
  bool m_selected;
  bool m_enabled;
  bool m_activated;
  CapEngine::Surface *m_pTextSurface;
  CapEngine::Vector m_position;
};

} // namespace CapEngine
#endif // BASICBUTTON_H
