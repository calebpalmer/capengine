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

  void drawLineBasicIncremental(int x0, int y0, int x1, int y1, CapEngine::Surface* surface, EdgePattern pattern=SolidEdge){
    // lock the surface for writing
    // TODO need to clip the coordinates to fit the screen
    bool drawPixel = true;
    int flipWrite = -1;
    if(pattern == StripedEdge)
      {
	flipWrite = basicStippleLength;
      }
    
    
    SDL_LockSurface(surface);

    float slope = (float)(y1 - y0) / (float)(x1 - x0);
    if(x0 > x1) { //swap positions . We want to always increment in positve x direction
      int temp;
      temp = x0;
      x0 = x1;
      x1 = temp;

      temp = y0;
      y0 = y1;
      y1 = temp;
    }

    if( slope > 1.0 || slope < -1.0){  // increment over y
      float lastx = x0;
      for(int i = y0; i <= y1;  i++){

	if(flipWrite != -1 && i % flipWrite == 0){
	  drawPixel = !drawPixel;
	}
	
	int y = i;
	int x;
	if(y == y0){
	  x = x0;
	}
	else {
	  lastx = lastx + (1.0f / slope);
	  x = lastx;
	}

	if(drawPixel){
	  writePixel(surface, x, y);
	}
      }
    }
    
    else{  // increment over x
      float lasty = y0;
      for(int i = x0; i <= x1; i++){

	if(flipWrite != -1 && i % flipWrite == 0){
	  drawPixel = !drawPixel;
	}

	int x = i;
	int y;
	if(x == x0){
	  y = y0;
	}
	else{
	  lasty = lasty + slope;
	  y = lasty;
	}
	if(drawPixel){
	  writePixel(surface, x, y);
	}
      }
    }

    // unlock surface
    SDL_UnlockSurface(surface);
  }

}

void CapEngine::drawLine(int x0, int y0, int x1, int y1, CapEngine::Surface* surface, EdgePattern pattern){
  // need to calculate intersection points if line needs to be clipped to fit surface
  if(surface == nullptr){
    throw CapEngineException("surface is null");
  }
  drawLineBasicIncremental(x0, y0, x1, y1, surface, pattern);
}

void CapEngine::writePixel(CapEngine::Surface* surface, int x, int y){

  // convert to screen coordinates
  int yNew = surface->h - y;

  // calculate offset into pixel buffer
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
    free(pixel);
    throw CapEngineException("Unsupported pixel format");
  }
    
  free(pixel);
}



