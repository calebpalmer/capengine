#ifndef CAPTYPES_H
#define CAPTYPES_H

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <map>
#include <memory>
#include <variant>

namespace CapEngine
{

using Surface = SDL_Surface;
using Texture = SDL_Texture;
using Renderer = SDL_Renderer;
using Rect = SDL_Rect;

using SurfacePtr = std::unique_ptr<Surface, decltype(&SDL_FreeSurface)>;
using TexturePtr = std::unique_ptr<Texture, decltype(&SDL_DestroyTexture)>;
using RendererPtr = std::unique_ptr<Renderer, decltype(&SDL_DestroyRenderer)>;
using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
//! Metadata value type
using MetadataType = std::variant<std::string, int, double>;

SurfacePtr getNullSurfacePtr();
TexturePtr getNullTexturePtr();
WindowPtr getNullWindowPtr();
RendererPtr getNullRendererPtr();

}  // namespace CapEngine

#endif // CAPTYPES_H
