#ifndef CAPENGINE_CONTROL_H
#define CAPENGINE_CONTROL_H

#include "widget.h"

namespace CapEngine { namespace UI {

		// This class is a UI control.  For now controls what the mouse is doing.
		class Control : public Widget {
		public:
			virtual ~Control() = default;

			virtual void setPosition(int x, int y) {};
			virtual void setSize(int width, int height) {};
			virtual void setHandled() {};
		};
		
}}

#endif // CAPENGINE_CONTROL_H
