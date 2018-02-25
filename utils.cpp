#include "utils.h"

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

}
