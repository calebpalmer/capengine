#include "eventutils.h"

namespace {

//! Returns the string represenation of a window event type.
/**
* \param event The window event
* \return The string representation
*/
std::string windowEventTypeToString(const SDL_WindowEvent& event){
	
}

} // anonymous namespace


namespace CapEngine {

//! Returns the string representation of an event type
/**
* \param event The event.
* \return string representation of the type of the event.
*/
std::string eventTypeToString(const SDL_Event &event){
	switch(event.type){
	case SDL_WINDOWEVENT:
		
	}
}

}
