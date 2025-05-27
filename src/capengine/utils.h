#ifndef CAPENGINE_UTILS_H
#define CAPENGINE_UTILS_H

#include <SDL2/SDL.h>

#include <cstdlib>
#include <optional>
#include <string>

namespace CapEngine
{

SDL_Rect expandRectToFit(const SDL_Rect &srcRect,
                         const SDL_Rect &containingRect);
std::optional<SDL_Rect> clipRect(const SDL_Rect &srcRect,
                                 const SDL_Rect &dstRect);
std::optional<SDL_Rect> intersectRects(const SDL_Rect &r1, const SDL_Rect &r2);
std::optional<std::string> getEnv(std::string const& in_varName);

} // namespace CapEngine

#endif // CAPENGINE_UTILS_H
