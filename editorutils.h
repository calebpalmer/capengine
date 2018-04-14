#ifndef CAPENGINE_EDITORUTILS_H
#define CAPENGINE_EDITORUTILS_H

#include "control.h"

#include <vector>
#include <memory>

namespace CapEngine { namespace UI {

		std::shared_ptr<std::vector<std::shared_ptr<UI::Control>>> getControlStack();

}}

#endif // CAPENGINE_EDITORUTILS_H
