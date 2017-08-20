#include "scanconvert.h"

#include "CapEngineException.h"
#include "VideoManager.h"
#include "locator.h"
#include "capcommon.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <cassert>


#include <boost/numeric/conversion/cast.hpp>

using namespace CapEngine;

namespace {

  Uint16 createUint16Pixel(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b){
    Uint16 pixel;
    pixel = ((r >> format->Rloss) << format->Rshift) +
      ((g >> format->Gloss) << format->Gshift) +
      ((b >> format->Bloss) << format->Bshift);

    return pixel;
  }

  Uint32 createUint32Pixel(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF){
    Uint32 pixel;
    pixel = ((r >> format->Rloss) << format->Rshift) +
      ((g >> format->Gloss) << format->Gshift) +
      ((b >> format->Bloss) << format->Bshift) +
      ((a >> format->Aloss) << format->Ashift);

    return pixel;
  }

  Uint32 createUint32Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF){
    Uint8 rshift, gshift, bshift, ashift;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rshift = 8*3;
    gshift = 8*2;
    bshift = 8*1;
    ashift = 8*0;
#else
    rshift = 8*0;
    gshift = 8*1;
    bshift = 8*2;
    ashift = 8*3;
#endif

    Uint32 pixel;
    pixel = (r << rshift) + (g << gshift) + (b << bshift) + (a << ashift);

    return pixel;
  }

  Uint32* createPixelBuffer(unsigned int size){
    unsigned int bufSize = sizeof(Uint32) * size;
    Uint32* buffer = (Uint32*)malloc(bufSize);
    memset(buffer, 0, bufSize);

    return buffer;
  }

  Surface* createSurfaceFromPixelBuffer(Uint32* pixels, unsigned int width,
					unsigned int height){
    const unsigned int pixelSize = 4; // bytes
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height,
						    pixelSize * 8, width * pixelSize, rmask, gmask,
						    bmask, amask);
    if(surface == nullptr){
      std::ostringstream errorStream;
      errorStream << "CreateRGBSurfaceFrom failed: " << SDL_GetError();
      throw CapEngineException(errorStream.str());
    }
    
    return surface;
  }

  void drawLineBasicIncremental(int x0, int y0, int x1, int y1, CapEngine::Surface* surface, EdgePattern pattern=EdgePattern::SolidEdge){
    // lock the surface for writing
    // TODO need to clip the coordinates to fit the screen
    bool drawPixel = true;
    int flipWrite = -1;
    if(pattern == EdgePattern::StripedEdge)
      {
	flipWrite = basicStippleLength;
      }
    
    
    sdlTry(std::bind(SDL_LockSurface, surface));

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

void fillRectangleBruteForce(Surface* surface, Rect rectangle, Colour colour){
  CAP_THROW_NULL(surface, "surface is null");
  CAP_THROW_NULL(Locator::videoManager, "VideoManager is null");
  
  VideoManager& vMan = *Locator::videoManager;
  int surfaceWidth = boost::numeric_cast<int>(vMan.getSurfaceWidth(surface));
  int surfaceHeight = boost::numeric_cast<int>(vMan.getSurfaceHeight(surface));

  sdlTry(std::bind(SDL_LockSurface, surface));
  for(int i = rectangle.x; i < rectangle.w; i++){
    for(int j = rectangle.y; j < rectangle.h; j++){
      if(i >= 0 && i < surfaceWidth && j >= 0 && j < surfaceHeight){
	//int screenJ = vMan.toScreenCoord(surface, j);
	writePixel(surface, i, j, colour);
      }
    }
  }
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
  int yNew = (surface->h - 1) - y;

  // calculate offset into pixel buffer of (x, y)
  // pitch is the length of a row in pixels
  int offset = (surface->pitch / surface->format->BytesPerPixel) * yNew + x;
	
  void* pixel;
  if(surface->format->BytesPerPixel == 2){
    pixel = malloc(2);
    *(Uint16*)pixel = createUint16Pixel(surface->format, scanR, scanG, scanB);
    ((Uint16*)surface->pixels)[offset] = *(Uint16*)pixel;
  }
  else if(surface->format->BytesPerPixel == 3){
    pixel = malloc(3); 
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

void CapEngine::writePixel(CapEngine::Surface* surface, int x, int y, CapEngine::Colour colour){
  // convert to screen coordinates
  int yNew = (surface->h - 1)- y;

  // calculate offset into pixel buffer of (x, y)
  // pitch is the length of a row in pixels
  int offset = (surface->pitch / surface->format->BytesPerPixel)
    * (yNew * surface->format->BytesPerPixel)
    + (x * surface->format->BytesPerPixel);

  assert(offset >= 0 && offset <= surface->pitch * surface ->h);
	
  void* pixel;
  if(surface->format->BytesPerPixel == 2){
    pixel = malloc(2);
    *(Uint16*)pixel = createUint16Pixel(surface->format, colour.m_r, colour.m_g, colour.m_g);
    ((Uint16*)surface->pixels)[offset / 2] = *(Uint16*)pixel;
  }
  else if(surface->format->BytesPerPixel == 3){
    pixel = malloc(3); 
    *(Uint32*)pixel = createUint32Pixel(surface->format, colour.m_r, colour.m_g, colour.m_b, colour.m_a);
    ((Uint32*)surface->pixels)[offset / 3] = *(Uint32*)pixel;
  }
  else if(surface->format->BytesPerPixel == 4){
    pixel = malloc(4);
    *(Uint32*)pixel = createUint32Pixel(surface->format, colour.m_r, colour.m_g, colour.m_b, colour.m_a);
    ((Uint32*)surface->pixels)[offset / 4] = *(Uint32*)pixel;
  }
  else{
    throw CapEngineException("Unsupported pixel format");
  }
    
  free(pixel);
}

void CapEngine::writePixel(Uint32* buffer, int x, int y, CapEngine::Colour colour, int bufWidth, int bufHeight){
  // convert to screen coordinates
  int yNew = (bufHeight - 1)- y;
  
  // calculate offset into pixel buffer of (x, y)
  // bufWidth is the length of a row in pixels
  int bytesPerPixel = 4;
  int offset = (bufWidth)
    * (yNew)
    + (x);


  assert(offset >= 0 && offset <= bufWidth * bufHeight * 4);
	
  void* pixel;
  pixel = malloc(bytesPerPixel);
  *(Uint32*)pixel = createUint32Pixel(colour.m_r, colour.m_g, colour.m_b);
  buffer[offset] = *(Uint32*)pixel;

  free(pixel);
}


Surface* CapEngine::createRectangle(int width, int height, Colour colour){
  if(Locator::videoManager == nullptr){
    throw CapEngineException("Locator::videoManager has not been set.");
  }

  Surface* surface = Locator::videoManager->createSurface(width, height);

  sdlTry(std::bind(SDL_LockSurface, surface));

  //std::cout << "Writing pixels to surface" << std::endl;
  
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      //std::cout << j << "," << i << std::endl;
      writePixel(surface, j, i, colour);
    }
  }

  SDL_UnlockSurface(surface);
  //Locator::logger->log("Pixels written to rectangle surface", Logger::CDEBUG, __FILE__, __LINE__);

  // //validate pixels
  // int buffsize = surface->pitch * surface->h;
  // int pixelSize = surface->pitch / surface->w;
  // for(int i = 0; i < buffsize; i += pixelSize){
  //   Uint32 value = ((Uint32*)surface->pixels)[i / pixelSize];
  //   int x = (i / pixelSize) % (surface->pitch / pixelSize);
  //   int y = (i / pixelSize) / (surface->pitch / pixelSize);
  //   std::cout << "Pixel at " << i << " at coordinate (" << x << "," << y <<  ") = " << value
  // 	      << " r: " << (value & surface->format->Rmask) << ", "
  // 	      << " g: " << (value & surface->format->Gmask) << ","
  // 	      << " b: " << (value & surface->format->Bmask) << ","
  // 	      << " a: " << (value & surface->format->Amask)
  // 	      << std::endl;
    
  // }

  //Locator::logger->log("Pixels validated", Logger::CDEBUG, __FILE__, __LINE__);
  
  return surface;
}

Surface* CapEngine::createRectangle2(int width, int height, Colour colour){
  if(Locator::videoManager == nullptr){
    throw CapEngineException("Locator::videoManager has not been set.");
  }

  unsigned int pixelSize = 4;  // bytes
  unsigned int bufSize = width * height * pixelSize;
  Uint32* pixels = createPixelBuffer(bufSize);

  //std::cout << "Writing pixels to surface" << std::endl;
  
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      //std::cout << j << "," << i << std::endl;
      writePixel(pixels, j, i, colour, width, height);
    }
  }
  //Locator::logger->log("Pixels written to rectangle surface", Logger::CDEBUG, __FILE__, __LINE__);

  //validate pixels
//   for(unsigned int i = 0; i < bufSize; i += pixelSize){
//     Uint32 value = ((Uint32*)pixels)[i];
//     int x = (i / pixelSize) % width;
//     int y = (i / pixelSize) / width;
//     if(value != 0){
//       Uint32 rmask, gmask, bmask, amask;      
// #if SDL_BYTEORDER == SDL_BIG_ENDIAN
//       rmask = 0xff000000;
//       gmask = 0x00ff0000;
//       bmask = 0x0000ff00;
//       amask = 0x000000ff;
// #else
//       rmask = 0x000000ff;
//       gmask = 0x0000ff00;
//       bmask = 0x00ff0000;
//       amask = 0xff000000;
// #endif
//       std::cout << "Pixel at " << i << " at coordinate (" << x << "," << y <<  ") = " << value
// 		<< " r: " << (value & rmask) << ", "
// 		<< " g: " << (value & gmask) << ","
// 		<< " b: " << (value & bmask) << ","
// 		<< " a: " << (value & amask)
// 		<< std::endl;
//     }
//  }

  //Locator::logger->log("Pixels validated", Logger::CDEBUG, __FILE__, __LINE__);

  Surface* surface = createSurfaceFromPixelBuffer(pixels, width, height);
  return surface;
}

Uint32 CapEngine::getPixel(const CapEngine::Surface* surface, int x, int y){

  // This was taken from http://sdl.beuc.net/sdl.wiki/Pixel_Accessblack
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

  switch(bpp) {
  case 1:
    return *p;
    break;

  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;

  case 4:
    return *(Uint32 *)p;
    break;

  default:
    return 0;       /* shouldn't happen, but avoids warnings */

  }
}

void CapEngine::getPixelComponents(const CapEngine::Surface* surface, int x, int y, Uint8* r, Uint8* g, Uint8* b, Uint8* a){
  Uint32 pixel = getPixel(surface, x, y);
  Uint32 rmask, gmask, bmask, amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
    *r = (pixel & rmask) >> 24;
    *g= (pixel & gmask) >> 16;
    *b= (pixel & bmask) >> 8;
    *a= (pixel & amask);
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
    *r = (pixel & rmask);
    *g= (pixel & gmask) >> 8;
    *b= (pixel & bmask) >> 16;
    *a= (pixel & amask) >> 24;
#endif

}


Pixel CapEngine::getPixelComponents(const CapEngine::Surface* surface, int x, int y){
  Uint8 r = 0x00;
  Uint8 g = 0x00;
  Uint8 b = 0x00;
  Uint8 a = 0x00;
  getPixelComponents(surface, x, y, &r, &g, &b, &a);

  return {r, g, b, a};
}

boost::optional<real> CapEngine::getSlopeAtPixel(const CapEngine::Surface* surface, int x, int y, Pixel solidPixel, int numNeighbours, bool above){
  auto pixelsEqual = [](const Pixel& lhs, const Pixel& rhs) -> bool {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
  };
  
  // if pixel at x,y is not a solid pixel then return nothing
  Pixel mainPixel = getPixelComponents(surface, x, y);
  if(mainPixel.r != solidPixel.r || mainPixel.g != solidPixel.g || mainPixel.b != solidPixel.b || mainPixel.a != solidPixel.a){
    return boost::optional<real>();
  }

  if(above){
    Pixel pixel = getPixelComponents(surface, x, y - 1);
    if(pixelsEqual(pixel, solidPixel)){
      return boost::optional<real>();
    }

    std::vector<Vector> points;

    VideoManager *vMan = Locator::videoManager;
    CAP_THROW_NULL(vMan, "VideoManager cannot be located");

    int surfaceWidth = vMan->getSurfaceWidth(surface);
    int surfaceHeight = vMan->getSurfaceHeight(surface);

    // starting from the middle to the left
    for(int i = x - 1; i >= x - numNeighbours; i--){
      if(i <= 0)
	break;
      
      int j = y;
      pixel = getPixelComponents(surface, i, y);
      //   if the pixel is solid, look up for the first non solid pixel until end of surface
      if( pixelsEqual(pixel, solidPixel)){
	while( j > 0 && pixelsEqual(getPixelComponents(surface, i, j--), solidPixel )) {}
	points.push_back(Vector(i, j));
      }
      //   if the pixel is not solid, look down each pixel until the first solid pixel is found, unless the pixel to the right is not solid
      else{
	while(j < surfaceHeight && !pixelsEqual(getPixelComponents(surface, i, j++), solidPixel) &&
	      pixelsEqual(getPixelComponents(surface, i + 1, j), solidPixel)) {}

	// pixel was found in current column
	if(pixelsEqual(getPixelComponents(surface, i, j), solidPixel)){
	  points.push_back(Vector(i, j));
	}

	// pixel was not found, use pixel to right if found
	else if(pixelsEqual(getPixelComponents(surface, i + 1, j), solidPixel)){
	  points.push_back(Vector(i + 1, j));
	}

	// pixel was not found to right either, so it must be one above this and to the right
	else{
	  if(!pixelsEqual(getPixelComponents(surface, i + i, j - i), solidPixel)){
	    std::stringstream error;
	    error << "Expecting solid pixel at (" << (i - 1) << ", " << (j - 1) << ")";
	    BOOST_THROW_EXCEPTION(CapEngineException(error.str()));
	  }
	  points.push_back(Vector(i + 1, j - 1));
	}
      }
    }

    // do the same from middle to right
    for(int i = x + 1; i < x + numNeighbours; i ++){
      if(i >= surfaceWidth - 1)
	break;

      int j = y;
      pixel = getPixelComponents(surface, i, y);
      //   if the pixel is solid, look up for the first non solid pixel until end of surface
      if( pixelsEqual(pixel, solidPixel)){
	while( j > 0 && pixelsEqual(getPixelComponents(surface, i, j--), solidPixel )) {}
	points.push_back(Vector(i, j));
      }

      //   if the pixel is not solid, look down each pixel until the first solid pixel is found, unless the pixel to the right is not solid
      else{
	while(j < surfaceHeight && !pixelsEqual(getPixelComponents(surface, i, j++), solidPixel) &&
	      pixelsEqual(getPixelComponents(surface, i - 1, j), solidPixel)) {}

	// pixel was found in current column
	if(pixelsEqual(getPixelComponents(surface, i, j), solidPixel)){
	  points.push_back(Vector(i, j));
	}

	// pixel was not found, use pixel to right if found
	else if(pixelsEqual(getPixelComponents(surface, i - 1, j), solidPixel)){
	  points.push_back(Vector(i - 1, j));
	}

	// pixel was not found to right either, so it must be one above this and to the right
	else{
	  if (!pixelsEqual(getPixelComponents(surface, i - i, j - i), solidPixel)){
	    std::stringstream error;
	    error << "Expecting solid pixel at (" << (i - 1) << ", " << (j - 1) << ")";
	    BOOST_THROW_EXCEPTION(CapEngineException(error.str()));
	  }
	  points.push_back(Vector(i - 1, j - 1));
	}

      }
      
    }

    // order the pixels by x and make sure only one point exists per x
    std::sort(points.begin(), points.end(),
	      [](const Vector& lhs, const Vector& rhs) { return lhs.x <= rhs.x; });
    std::unique(points.begin(), points.end(),
	      [](const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x; });		


    // get the average of all the slopes
    real sumSlope = 0.0;
    for(unsigned int i = 0; i < points.size() - 1; i++){
      CAP_THROW_NULL(Locator::videoManager, "Cannot locate VideoManager");

      int y1 = Locator::videoManager->fromScreenCoord(surface, points[i].y);
      int y2 = Locator::videoManager->fromScreenCoord(surface, points[i+1].y);
      int x1 = points[i].x;
      int x2 = points[i+1].x;
      
      sumSlope += (y2 - y1) / (x2 - x1);
    }

    return boost::optional<real>(sumSlope / points.size() - 1);
  }

  // below
  else{
    BOOST_THROW_EXCEPTION(CapEngineException("Not implemented"));
  }
  
}

void CapEngine::fillRectangle(Surface* surface, Rect rectangle, Colour colour){
  fillRectangleBruteForce(surface, rectangle, colour);
}
