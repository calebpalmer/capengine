#ifndef CAPENGINE_UIPROPERTYVISITOR_H
#define CAPENGINE_UIPROERTYVISITOR_H

#include "propertyvisitor.h"
#include "widget.h"

#include <optional>

namespace CapEngine
{
namespace UI
{

// Property visitor for building UI widgets
class UIPropertyVisitor final : public PropertyVisitor
{
public:
  ~UIPropertyVisitor() override = default;

  void visitString(Property &property) override;
  void visitInt64(Property &property) override;
  void visitUInt64(Property &property) override;
  void visitInt32(Property &property) override;
  void visitUInt32(Property &property) override;
  void visitFloat32(Property &property) override;
  void visitFloat64(Property &property) override;
  void visitList(Property &property) override;

  std::optional<std::shared_ptr<Widget>> getWidget();

private:
  //! The widget
  std::shared_ptr<Widget> m_pWidget;
};
} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_UIPROPERTYVISITOR_H
