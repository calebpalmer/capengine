#ifndef CAPENGINE_SDLUTILS_H
#define CAPENGINE_SDLUTILS_H

#include "CapEngineException.h"

#include <SDL2/SDL.h>

bool operator<(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs);
bool operator>(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs);
bool operator<=(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs);
bool operator>=(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs);

#endif /* CAPENGINE_SDLUTILS_H */
