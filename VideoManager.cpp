//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include "VideoManager.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <SDL/SDL_image.h>

#include "logger.h"
#include "EventDispatcher.h"

using namespace std;
using namespace CapEngine;

bool VideoManager::instantiated = false;

VideoManager::VideoManager() : up_fontManager(new FontManager()), showFPS(false) {
  assert(instantiated == false);
  instantiated = true;
  mainSurface = nullptr;
  initialized = false;
}

VideoManager::VideoManager(Logger* loggerIn) : up_fontManager(new FontManager()), showFPS(false) {
  assert(instantiated == false);
  instantiated = true;
  mainSurface = nullptr;
  initialized = false;
  logger = loggerIn;
}

Surface* VideoManager::loadImage(string filePath) const{
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted=IMG_Init(flags);

  if( (initted & flags) != flags) {
    ostringstream error_message;
    error_message <<"could not init SDL_Image" << endl;
    error_message<<"Reason: " << IMG_GetError() << endl;
    throw CapEngineException(error_message.str());
  }


  SDL_Surface* tempSurface = NULL;
  SDL_Surface* optimizedSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());

  ostringstream logString;
  logString << "Loaded surface from file "  << filePath;
  logger->log(logString.str(), Logger::CDEBUG);

  if (tempSurface == NULL){
    cerr << "Unable to load surface" << endl;
    cerr << "SDL Error: " << SDL_GetError() << endl;
    return tempSurface;
  }

  setColorKey(tempSurface);
  optimizedSurface = SDL_DisplayFormatAlpha(tempSurface);
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
  
  Uint32 flags;
  if(screenConfig.opengl){
    flags = SDL_OPENGL | SDL_RESIZABLE;
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);
 
    // These cause error "Couldn't find matching GLX visual" 
    // when calling SDL_SetVideoMode
    //SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
    //SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    //SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
    //SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  }
  else{
    flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_RESIZABLE;
  }

  mainSurface = SDL_SetVideoMode(screenConfig.width, screenConfig.height, screenConfig.pDepth, flags);
  //mainSurface = SDL_SetVideoMode(screenConfig.width, screenConfig.height, screenConfig.pDepth, SDL_SWSURFACE);
  if(mainSurface == NULL){
    ostringstream errorMsg;
    errorMsg << "Error initializing window: " << SDL_GetError() << endl;
    logger->log(errorMsg.str(), Logger::CERROR);
    shutdown();
  }

  currentScreenConfig = screenConfig;

  // initialise logger if necessary
  if(logger == nullptr){
    logger = new Logger();
  }

  initialized = true;
}

void VideoManager::shutdown(){
  SDL_Quit();
}

void VideoManager::drawScreen(){
  if(showFPS){
    string sFPS = to_string(fps);
    int fontSize = 14;
    Surface* fpsSurface = up_fontManager->getTextSurface(ttfFontPath, sFPS, fontSize);
    
     int x = 15;
     int y = 15;
    drawSurface(x, y, fpsSurface);
  }
  if(currentScreenConfig.opengl){
    SDL_GL_SwapBuffers();
  }
  else{
    SDL_Flip(mainSurface);
  }
  Time currentTime;
  double elapsedTime = currentTime.subtractTime(&lastRenderTime);
  lastRenderTime = currentTime;
  fps = 1 / (elapsedTime * 0.001);
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
  logger->log(logString.str(), Logger::CDEBUG);
  
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

void VideoManager::setReshapeFunc(void (*func)(int x, int y)){
  reshapeFunc = func;
}

 void VideoManager::callReshapeFunc(int w, int h){
   if(reshapeFunc == nullptr){
     throw CapEngineException("No reshape function set"); 
   }
   if(currentScreenConfig.opengl){
     reshapeFunc(w, h);
  }
}

void VideoManager::displayFPS(bool on, const string& ttfFontPath){
  showFPS = on;
  this->ttfFontPath = ttfFontPath;
}
