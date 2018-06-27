#include "utils.h"
#include "collision.h"

#include <algorithm>

namespace CapEngine {

//! Expands a Rectangle to fit another rectangle maintaining aspect ration.
/** 
 \param srcRect - The rectangle to expand/shrink.
 \param containingRect - The rectangle to fit into
 \return A new rectangle expanded to fit.
*/
SDL_Rect expandRectToFit(const SDL_Rect &srcRect, const SDL_Rect &containingRect){

	SDL_Rect newRect = {containingRect.x, containingRect.y, 0, 0};
	
	float containingAspectRatio = containingRect.h != 0 
 		? static_cast<double>(containingRect.w) / static_cast<double>(containingRect.h)
		: 1.0;
		
	float srcAspectRatio = srcRect.h != 0
		? static_cast<double>(srcRect.w) / static_cast<double>(srcRect.h)
		: 1.0;

	if(srcAspectRatio > containingAspectRatio){
		newRect.w = containingRect.w;
		newRect.h = static_cast<double>(srcRect.h) *
			(static_cast<double>(newRect.w) / static_cast<double>(srcRect.w));

		// center it vertically
		newRect.y = containingRect.y + ((containingRect.h - newRect.h) / 2);
	}
	else{
		newRect.h = containingRect.h;
		newRect.w = static_cast<double>(srcRect.w) *
			(static_cast<double>(newRect.h) / static_cast<double>(srcRect.h));

		// center horizontally
		newRect.x = containingRect.x + ((containingRect.w - newRect.w) / 2);
	}

	return newRect;
}


//! Clip a rectangle into another.
/** 
 \param srcRect - The rect to clip.
 \param dstRect - The rect to clip to.
 \return - The clipped rect.
*/
std::optional<SDL_Rect> clipRect(const SDL_Rect &srcRect, const SDL_Rect &dstRect){
	
	if(detectMBRCollision(srcRect, dstRect) == COLLISION_NONE)
		return std::nullopt;

	SDL_Rect clippedRect;
	clippedRect.x = srcRect.x < dstRect.x ? dstRect.x : srcRect.x;
	clippedRect.y = srcRect.y < dstRect.y ? dstRect.y : srcRect.y;

	if(srcRect.x + srcRect.w > dstRect.x + dstRect.w)
		clippedRect.w = dstRect.x + dstRect.w - clippedRect.x;

	else
		clippedRect.w = srcRect.w;

	if(srcRect.y + srcRect.h > dstRect.y + dstRect.h)
		clippedRect.h = dstRect.y + dstRect.h - clippedRect.y;
	else
		clippedRect.h = srcRect.h;


	return clippedRect;
}



//! Returns the intersection of two rectangles
/** 
 \param r1 - a rect
 \param r2 - a rect
 \return - The intersection
*/
std::optional<SDL_Rect> intersectRects(const SDL_Rect &r1, const SDL_Rect &r2){
	auto relation = MBRRelate(r1, r2);

	if(relation != OUTSIDE){
		int l = std::max(r1.x, r2.x);
		int r = std::min(r1.x + r1.w, r2.x + r2.w);
		int b = std::max(r1.y, r2.y);
		int t = std::min(r1.y + r1.h, r2.y + r2.h);
	
		SDL_Rect r3 = {l, b, r - l + 1, t - b + 1};
		return r3;
	}

	return std::nullopt;
}

}
