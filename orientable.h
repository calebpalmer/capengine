#ifndef CAPENGINE_ORIENTABLE_H
#define CAPENGINE_ORIENTABLE_H

#include <set>

namespace CapEngine { namespace UI {

		//! Abstract class for Widgets that can have alignment and padding.
		class Orientable {
		public:
			//! enum representing horizontal alignment
			enum class HorizontalAlignment {
				Left = 0,
				Center,
				Right
			};

			//! enum representing vertical alignment
			enum class VerticalAlignment {
				Top = 0,
				Center,
				Bottom
			};

			//! The location in a box
			enum class Location {
				Left=0,
			  Top,
				Right,
				Bottom
			};
			
		public:
			virtual ~Orientable() = default;

			virtual void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);
			virtual void setVerticalAlignment(VerticalAlignment verticalAlignment);
			virtual void setPadding(int padding, std::set<Location> sides = { Location::Left, Location::Right, Location::Top, Location::Bottom });

		protected:
			struct Padding {
				int leftPadding;
				int rightPadding;
				int topPadding;
				int bottomPadding;
			};

		protected:
			//! The horizontal alignment
			HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;
 			//! The vertical alignment
			VerticalAlignment m_verticalAlignment = VerticalAlignment::Center;
			//! The padding.
			Padding m_padding;
		};

}}

#endif // CAPENGINE_ORIENTABLE_H
