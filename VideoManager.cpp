//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include "VideoManager.h"

#include <iostream>
#include <sstream>

#include <SDL/SDL_image.h>

#include "logger.h"

using namespace std;
using namespace CapEngine;

VideoManager* VideoManager::instance = nullptr;

VideoManager::VideoManager(){
  mainSurface = nullptr;
  initialized = false;
}

VideoManager& VideoManager::getInstance(){
  if(instance == nullptr){
    unique_ptr<VideoManager> sp_VideoManager(new VideoManager);
    instance = sp_VideoManager.release();
  }
  return *instance;
}

Surface* VideoManager::loadImage(string filePath) const{
  SDL_Surface* tempSurface = NULL;
  SDL_Surface* optimizedSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());

  ostringstream logString;
  logString << "Loaded surface from file "  << filePath;
  Logger::getInstance().log(logString.str(), Logger::CDEBUG);

  if (tempSurface == NULL){
    cerr << "Unable to load surface" << endl;
    cerr << "SDL Error: " << SDL_GetError() << endl;
    return tempSurface;
  }

  setColorKey(tempSurface);
  optimizedSurface = SDL_DisplayFormat(tempSurface);
  if(optimizedSurface == 0){
    cerr << "Unable to get optimized surface" << endl;
    cerr << "SDL Error: " << SDL_GetError() << endl;
    return tempSurface;
  }
  
  SDL_FreeSurface(tempSurface);
  
  return optimizedSurface;
  
}

void VideoManager::drawSurface(int x, int y, Surface* source, Rect* rect) const{
  
  SDL_Rect location;
  location.x = x;
  location.y = y;

  SDL_BlitSurface(source, rect, mainSurface, &location);
  
}

void VideoManager::initSystem(Screen_t screenConfig){
  mainSurface = NULL;
  
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == -1){
    cerr << "Unable to initialize SDL. Shutting down." << endl;
    shutdown();
  }

  mainSurface = SDL_SetVideoMode(screenConfig.width, screenConfig.height, screenConfig.pDepth, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
  //mainSurface = SDL_SetVideoMode(screenConfig.width, screenConfig.height, screenConfig.pDepth, SDL_SWSURFACE);
  if(mainSurface == NULL){
    cerr << "Error initializing window" << endl;
    shutdown();
  }

  initialized = true;
}

void VideoManager::shutdown(){
  SDL_Quit();
}

void VideoManager::drawScreen(){
  SDL_Flip(mainSurface);
}

void VideoManager::getWindowResolution(int* width, int* height) const{
  *width = defaultScreen.width;
  *height = defaultScreen.height;
}

//! get the width of given surface
/*!
  \param surface
*/
real VideoManager::getSurfaceWidth(const Surface* surface) const{
  CAP_THROW_NULL(surface, "surface passed to getSurfaceWidth is null");
  return surface->w;
}

//! get the height of given surface
/*!
  \param surface
 */
real VideoManager::getSurfaceHeight(const Surface* surface) const{
    CAP_THROW_NULL(surface, "surface passed to getSurfaceHeight is null");
    return surface->h;
}

//! close a surface openned by the VideoManager
/*!
  \param surface
  \li the surface to close
 */
void VideoManager::closeSurface(Surface* surface) const{
  SDL_FreeSurface(surface);
}

//! Set the color key for the image
/*! by default it is 0x00ffff
 */
void VideoManager::setColorKey(Surface* surface) const{
  CAP_THROW_NULL(surface, "Cannot set color key on a null surface");
  
  if(SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF)) == -1){
    throw new CapEngineException("Error setting the color key of the surface");
  }
}

Surface* VideoManager::createSurface(int width, int height){
  // code taken from http://www.libsdl.org/docs/html/sdlcreatergbsurface.html
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  SDL_Surface *surface;
  Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
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
    
  surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32,
				 rmask, gmask, bmask, amask);
  if(surface == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }

  setColorKey(surface);

  SDL_Surface* optimizedSurface = NULL;
  optimizedSurface = SDL_DisplayFormat(surface);
  if(optimizedSurface == 0){
    cerr << "Unable to get optimized surface" << endl;
    cerr << "SDL Error: " << SDL_GetError() << endl;
    return surface;
  }
  
  SDL_FreeSurface(surface);
  
  ostringstream logString;
  logString << "Created new surface with dimensions " << width << "x" << height;
  Logger::getInstance().log(logString.str(), Logger::CDEBUG);
  
  return optimizedSurface;
}

void VideoManager::blitSurface(Surface& sourceSurface, int srcX, int srcY, int sourceWidth, int sourceHeight, Surface& destSurface, int x, int y){
  SDL_Rect srcLocation;
  srcLocation.x = srcX;
  srcLocation.y = srcY;
  srcLocation.w = sourceWidth;
  srcLocation.h = sourceHeight;

  SDL_Rect dstLocation;
  dstLocation.x = x;
  dstLocation.y = y;

  SDL_BlitSurface(&sourceSurface, &srcLocation, &destSurface, &dstLocation);
}
