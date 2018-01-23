#ifndef EVENTUTILS_H
#define EVENTUTILS_H


#include <string>
#include <SDL2/SDL.h>

namespace CapEngine {

	std::string eventTypeToString(const SDL_Event &event);
	
}

#endif // EVENTUTILS_H
