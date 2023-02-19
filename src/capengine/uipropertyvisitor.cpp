#include "uipropertyvisitor.h"

#include "label.h"
#include "linearlayout.h"
#include "listproperty.h"
#include "textbox.h"

namespace CapEngine
{
namespace UI
{

namespace
{

//! Creates a label and textbox widget for a property.
/**
 \param property
   The property.
 \return
   The widget.
*/
std::shared_ptr<Widget> genericLabelAndTextWidget(Property &property)
{
  std::shared_ptr<LinearLayout> pLayout =
      LinearLayout::create(LinearLayout::Orientation::Horizontal);
  assert(pLayout != nullptr);

  // add the label
  std::shared_ptr<Label> pLabel = Label::create(property.getName());
  assert(pLabel != nullptr);

  pLayout->addWidget(std::move(pLabel));

  // add the textbox
  std::optional<std::string> maybeVal = property.getValueAs<std::string>();
  std::string value = maybeVal != std::nullopt ? *maybeVal : std::string();

  std::shared_ptr<TextBox> pTextBox = TextBox::create(value);
  assert(pTextBox != nullptr);

  pLayout->addWidget(std::move(pTextBox));

  return pLayout;
}

} // namespace

//! \copydoc PropertVistor::visitString
void UIPropertyVisitor::visitString(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitInt64
void UIPropertyVisitor::visitInt64(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitUInt64
void UIPropertyVisitor::visitUInt64(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitInt32
void UIPropertyVisitor::visitInt32(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitInt4
void UIPropertyVisitor::visitUInt32(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitFloat64
void UIPropertyVisitor::visitFloat64(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitFloat32
void UIPropertyVisitor::visitFloat32(Property &property)
{
  m_pWidget = genericLabelAndTextWidget(property);
}

//! \copydoc PropertyVisitory::visitFloat32
void UIPropertyVisitor::visitList(Property &property)
{
  std::optional<ListProperty> maybeListProperty =
      property.getValueAs<ListProperty>();
  assert(maybeListProperty != std::nullopt);

  std::shared_ptr<LinearLayout> pLayout =
      LinearLayout::create(LinearLayout::Orientation::Vertical);
  assert(pLayout != nullptr);

  for (auto &&subProperty : *maybeListProperty) {
    UIPropertyVisitor visitor;
    subProperty.accept(visitor);

    std::optional<std::shared_ptr<Widget>> maybeWidget = visitor.getWidget();
    assert(maybeWidget != std::nullopt);
    assert(*maybeWidget != nullptr);

    pLayout->addWidget(*maybeWidget);
  }

  m_pWidget = std::move(pLayout);
}

//! Get the widget.
/**
 \return
   The widget.
*/
std::optional<std::shared_ptr<Widget>> UIPropertyVisitor::getWidget()
{
  return m_pWidget;
}

} // namespace UI
} // namespace CapEngine
