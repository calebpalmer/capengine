#include "orientable.h"

namespace CapEngine
{
namespace UI
{

//! Sets the horizontal alignment;
/**
                \param horizontalalignment - The desired horizontal algignment
*/
void Orientable::setHorizontalAlignment(HorizontalAlignment horizontalAlignment)
{
  m_horizontalAlignment = horizontalAlignment;
}

//! Sets the vertical alignment;
/**
                \param verticalalignment - The desired vertical algignment
*/
void Orientable::setVerticalAlignment(VerticalAlignment verticalAlignment)
{
  m_verticalAlignment = verticalAlignment;
}

//! Sets the padding on the Orientable object.
/**
 \param padding - The amount of padding (pixels) to set.
 \param sides - The sides to set the padding on.
*/
void Orientable::setPadding(int padding, std::set<Location> sides)
{
  for (auto &&side : sides) {
    switch (side) {
    case Location::Left:
      m_padding.leftPadding = padding;
      break;

    case Location::Top:
      m_padding.topPadding = padding;
      break;

    case Location::Right:
      m_padding.rightPadding = padding;
      break;

    case Location::Bottom:
      m_padding.bottomPadding = padding;
      break;
    }
  }
}

} // namespace UI
} // namespace CapEngine
