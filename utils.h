#ifndef CAPENGINE_UTILS_H
#define CAPENGINE_UTILS_H

#include <SDL2/SDL.h>

//#include <boost/optional.hpp>
#include <optional>

namespace CapEngine
{

SDL_Rect expandRectToFit(const SDL_Rect &srcRect,
                         const SDL_Rect &containingRect);
std::optional<SDL_Rect> clipRect(const SDL_Rect &srcRect,
                                 const SDL_Rect &dstRect);
std::optional<SDL_Rect> intersectRects(const SDL_Rect &r1, const SDL_Rect &r2);

} // namespace CapEngine

#endif // CAPENGINE_UTILS_H
