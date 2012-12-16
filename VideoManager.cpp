//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include <iostream>
#include "VideoManager.h"

#include <SDL/SDL_image.h>

using namespace std;
using namespace CapEngine;

VideoManager::VideoManager(){
  mainSurface = 0;
}

Surface* VideoManager::loadImage(string filePath) const{
  SDL_Surface* tempSurface = NULL;
  SDL_Surface* optimizedSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());

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
