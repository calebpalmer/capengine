#include "scanconvert.h"
#include "CapEngineException.h"

#include <SDL/SDL.h>
#include <iostream>

using namespace CapEngine;

namespace {

  Uint16 createUint16Pixel(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b){
    Uint16 pixel;
    pixel = ((r >> format->Rloss) << format->Rshift) +
      ((g >> format->Gloss) << format->Gshift) +
      ((b >> format->Bloss) << format->Bshift);

    return pixel;
  }

  Uint32 createUint32Pixel(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b){
    Uint32 pixel;
    pixel = ((r >> format->Rloss) << format->Rshift) +
      ((g >> format->Gloss) << format->Gshift) +
      ((b >> format->Bloss) << format->Bshift);

    return pixel;
  }

  void writePixel(CapEngine::Surface* surface, int x, int y){

    // TODO convert cartesian coordinates to screen coordinates

    //int offset = surface->pitch * y + x;
    // convert to screen coordinates
    int yNew = surface->h - y;
    //int yNew = y;
    int offset = (surface->pitch / surface->format->BytesPerPixel) * yNew + x;
	
    void* pixel;
    if(surface->format->BytesPerPixel == 2){
      pixel = malloc(2);
      *(Uint16*)pixel = createUint16Pixel(surface->format, scanR, scanG, scanB);
      ((Uint16*)surface->pixels)[offset] = *(Uint16*)pixel;
    }
    else if(surface->format->BytesPerPixel == 3){
      pixel = malloc(4);
      *(Uint32*)pixel = createUint32Pixel(surface->format, scanR, scanG, scanB);
      ((Uint32*)surface->pixels)[offset] = *(Uint32*)pixel;
    }
    else if(surface->format->BytesPerPixel == 4){
      pixel = malloc(4);
      *(Uint32*)pixel = createUint32Pixel(surface->format, scanR, scanG, scanB);
      ((Uint32*)surface->pixels)[offset] = *(Uint32*)pixel;
    }
    else{
      throw CapEngineException("Unsupported pixel format");
    }
    
    free(pixel);
  }

  void drawLineBasicIncremental(int x0, int y0, int x1, int y1, CapEngine::Surface* surface){
    // lock the surface for writing
    // TODO need to clip the coordinates to fit the screen
    
    SDL_LockSurface(surface);

    float slope = (float)(y1 - y0) / (float)(x1 - x0);

    if( slope > 1.0 || slope < -1.0){  // increment over y
      float lastx = x0;
      for(int i = y0; i <= y1;  i++){
	int y = i;
	int x;
	if(y == y0){
	  x = x0;
	}
	else {
	  lastx = lastx + (1.0f / slope);
	  x = lastx;
	}
	
	writePixel(surface, x, y);
      }
    }
    
    else{  // increment over x
      float lasty = y0;
      for(int i = x0; i <= x1; i++){
	int x = i;
	int y;
	if(x == x0){
	  y = y0;
	}
	else{
	  lasty = lasty + slope;
	  y = lasty;
	}
	writePixel(surface, x, y);
      }
    }

    // unlock surface
    SDL_UnlockSurface(surface);
  }

}

void CapEngine::drawLine(int x0, int y0, int x1, int y1, CapEngine::Surface* surface){
  // need to calculate intersection points if line needs to be clipped to fit surface
  if(surface == nullptr){
    throw CapEngineException("surface is null");
  }
  drawLineBasicIncremental(x0, y0, x1, y1, surface);
}

