#ifndef CAPENGINE_UTILS_H
#define CAPENGINE_UTILS_H

#include <SDL2/SDL.h>

namespace CapEngine {

	SDL_Rect expandRectToFit(const SDL_Rect &srcRect, const SDL_Rect &containingRect);
	SDL_Rect clipRect(const SDL_Rect &srcRect, const SDL_Rect &dstRect);

}

#endif // CAPENGINE_UTILS_H
