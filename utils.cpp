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
	
	float containingAspectRatio = containingRect.w / containingRect.h;
	float srcAspectRatio = srcRect.w / srcRect.h;

	// if they're both wide, stretch to fit width
	if(containingAspectRatio >= 1.0 && srcAspectRatio >= 1.0){
		newRect.w = containingRect.w;
		newRect.h = srcRect.h *
			(static_cast<double>(newRect.w) / static_cast<double>(srcRect.w));

		// center it vertically
		newRect.y = containingRect.y + ((containingRect.h - newRect.h) / 2);
	}

	// if they're both narrows, streth to fit height
	else if(containingAspectRatio < 1.0 && srcAspectRatio < 1.0){
		newRect.h = containingRect.h;
		newRect.w = srcRect.w *
			(static_cast<double>(newRect.h) / static_cast<double>(srcRect.h));

		// center horizontally
		newRect.x = containingRect.x + ((containingRect.w - newRect.w) / 2);
	}

	// if src is wide and dst is narrow, stretch to fit width
	else if(srcAspectRatio >= 1.0 && containingAspectRatio < 1.0){
		newRect.w = containingRect.w;
		newRect.h = srcRect.h *
			(static_cast<double>(newRect.w) / static_cast<double>(srcRect.w));

		// center it vertically
		newRect.y = containingRect.y + ((containingRect.h - newRect.h) / 2);
	}

	// if src is narrow and dst is wide, strech to fit height
	else if(srcAspectRatio < 1.0 && containingAspectRatio >= 1.0){
		newRect.h = containingRect.h;
		newRect.w = srcRect.w *
			(static_cast<double>(newRect.h) / static_cast<double>(srcRect.h));

		// center horizontally
		newRect.x = containingRect.x + ((containingRect.w - newRect.w) / 2);
	}

	return newRect;
}

}
