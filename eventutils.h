#ifndef EVENTUTILS_H
#define EVENTUTILS_H

#include <SDL2/SDL.h>
#include <string>

namespace CapEngine
{

std::string eventTypeToString(const SDL_Event &event);

}

#endif // EVENTUTILS_H
