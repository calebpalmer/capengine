#include "captypes.h"

namespace CapEngine {


//! return a null SurfacePtr
/** 
* \return A null SurfacePtr
*/
SurfacePtr getNullSurfacePtr(){
	SurfacePtr nullSurfacePtr(nullptr, [](SDL_Surface*) {});
	return std::move(nullSurfacePtr);
}


//! return a null TexturePtr
/** 
* \return a null TexturePtr
*/
TexturePtr getNullTexturePtr(){
	TexturePtr nullTexturePtr(nullptr, [](SDL_Texture*) {});
	return std::move(nullTexturePtr);
}

} // namespace CapEngine
