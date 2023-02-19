#ifndef CAPTYPES_H
#define CAPTYPES_H

#include <SDL2/SDL.h>
#include <map>
#include <memory>
#include <variant>

namespace CapEngine {

typedef float real;

typedef SDL_Surface Surface;
typedef SDL_Texture Texture;
//  typedef SDL_Window Window;
// typedef SDL_Renderer Renderer;
typedef SDL_Rect Rect;

typedef std::unique_ptr<Surface, decltype(&SDL_FreeSurface)> SurfacePtr;
typedef std::unique_ptr<Texture, decltype(&SDL_DestroyTexture)> TexturePtr;

//! Metadata value type
using MetadataType = std::variant<std::string, int, double>;

SurfacePtr getNullSurfacePtr();
TexturePtr getNullTexturePtr();

} // namespace CapEngine

#endif // CAPTYPES_H
