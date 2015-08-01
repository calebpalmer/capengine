//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include "VideoManager.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <SDL2/SDL_image.h>

#include "logger.h"
#include "EventDispatcher.h"

using namespace std;
using namespace CapEngine;

bool VideoManager::instantiated = false;

VideoManager::VideoManager() : up_fontManager(new FontManager()), showFPS(false)
				  , logger(nullptr), m_pWindow(nullptr), m_pRenderer(nullptr)
			     , initialized(false)
{
  assert(instantiated == false);
  instantiated = true;
  logger = new Logger();
}

VideoManager::VideoManager(Logger* loggerIn) : up_fontManager(new FontManager()), showFPS(false)
					     , m_pWindow(nullptr), m_pRenderer(nullptr), logger(loggerIn), initialized(false)
{
  assert(instantiated == false);
  instantiated = true;
  initialized = false;
}

void VideoManager::initSystem(Screen_t screenConfig){
  currentScreenConfig = screenConfig;
  Uint32 flags = 0;
  
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
    ostringstream errorMsg;
    errorMsg << "Unable to initialize SDL. Shutting down." << endl;
    logger->log(errorMsg.str(), Logger::CERROR);
    shutdown();
  }
  
  if(screenConfig.opengl){
    throw CapEngineException("OpenGL not implemented");
  }

  else{
    if(screenConfig.fullScreen){
      flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
      m_pWindow = SDL_CreateWindow("CapEngine", SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED, 0, 0, flags);
    
    }
    else{
      m_pWindow = SDL_CreateWindow("CapEngine", SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED, screenConfig.width, screenConfig.height, flags);
    }

    if(m_pWindow == nullptr){
      ostringstream errorStream;
      errorStream << "Error creating window: " << SDL_GetError();
      throw CapEngineException(errorStream.str());
    }

    // Now create the 2d Renderer if OpenGL is not being used
    if( !screenConfig.opengl ){
      m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
      if(m_pRenderer == nullptr){
	ostringstream errorStream;
	errorStream << "Error creating renderer:  " << SDL_GetError();

	throw CapEngineException(errorStream.str());
      }
      if(screenConfig.fullScreen){
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(m_pRenderer, screenConfig.width, screenConfig.height);
      }
      SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    }
  }
}

Texture* VideoManager::createTextureFromSurface(Surface* surface){
  // Create Texture from Surface
  SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);
  if( texture == nullptr ){
    ostringstream errorMsg;
    errorMsg << "Unable to load texture from surface " << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }

  return texture;
}

Texture* VideoManager::loadImage(string filePath) const{
  // Try to initialise SDL_Image
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if( (initted & flags) != flags) {
    ostringstream error_message;
    error_message <<"could not init SDL_Image" << endl;
    error_message<<"Reason: " << IMG_GetError() << endl;
    throw CapEngineException(error_message.str());
  }

  // Load Surface
  SDL_Surface* tempSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());
  if(tempSurface == nullptr){
    ostringstream errorMsg;
    errorMsg << "Unable to load surface " << filePath << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }
  ostringstream logString;
  logString << "Loaded surface from file "  << filePath;
  logger->log(logString.str(), Logger::CDEBUG);

  setColorKey(tempSurface);

  // Create Texture from Surface
  SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
  if( texture == nullptr ){
    ostringstream errorMsg;
    errorMsg << "Unable to load texture from file " << filePath << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }

  SDL_FreeSurface(tempSurface);
  return texture;
}

void VideoManager::drawTexture(int x, int y, Texture* texture, Rect* srcRect) const{
  
  SDL_Rect dstRect;
  dstRect.x = x;
  dstRect.y = y;

  int result = SDL_RenderCopy(m_pRenderer, texture, srcRect, &dstRect);
  if(result != 0){
    logger->log("Unable to render texture", Logger::CERROR);
  }
  
}

void VideoManager::drawTexture(Texture* texture, Rect* srcRect, Rect* dstRect) const{
  SDL_RenderCopy(m_pRenderer, texture, srcRect, dstRect);
}

void VideoManager::shutdown(){
  SDL_DestroyRenderer(m_pRenderer);
  SDL_DestroyWindow(m_pWindow);
  SDL_Quit();
  instantiated = false;
}

void VideoManager::clearScreen(){
  // Clear Screen
  SDL_RenderClear(m_pRenderer);

  // draw rect to clear everything
  SDL_Rect rectangle;
  rectangle.x = 0;
  rectangle.y = 0;
  rectangle.w = currentScreenConfig.width;
  rectangle.h = currentScreenConfig.height;
  SDL_RenderFillRect(m_pRenderer, &rectangle);
}

void VideoManager::drawScreen(){
  // Render FPS if turned on
  if(showFPS){
    string sFPS = to_string(fps);
    int fontSize = 14;
    Surface* fpsSurface = up_fontManager->getTextSurface(ttfFontPath, sFPS, fontSize, fpsColourR, fpsColourG, fpsColourB);
    Texture* fpsTexture = SDL_CreateTextureFromSurface(m_pRenderer, fpsSurface);
    SDL_FreeSurface(fpsSurface);
    
    int x = 15;
    int y = 15;
    drawTexture(x, y, fpsTexture);
    this->closeTexture(fpsTexture);
  }

  // draw the screen
  if(currentScreenConfig.opengl){
    SDL_GL_SwapWindow(m_pWindow);
  }
  else{
    SDL_RenderPresent(m_pRenderer);
  }
  
  // Update fps variables
  Time currentTime;
  double elapsedTime = currentTime.subtractTime(&lastRenderTime);
  lastRenderTime = currentTime;
  fps = 1 / (elapsedTime * 0.001);
}

void VideoManager::getWindowResolution(int* width, int* height) const{
  *width = currentScreenConfig.width;
  *height = currentScreenConfig.height;
}

int VideoManager::getWindowWidth() const{
  return currentScreenConfig.width;
}

int VideoManager::getWindowHeight() const{
  return currentScreenConfig.height;
}

//! get the width of given texture
/*!
  \param texture
*/
real VideoManager::getTextureWidth(Texture* texture) const{
  CAP_THROW_NULL(texture, "Texture is null");
  int w;
  int result = SDL_QueryTexture(texture, nullptr, nullptr, &w, nullptr);
  if(result < 0){
    ostringstream error;
    error << "Unable to get texture width" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR);
    throw CapEngineException(error.str());
  }
  return w;
}

//! get the height of given texture
/*!
  \param texture
 */
real VideoManager::getTextureHeight(Texture* texture) const{
    CAP_THROW_NULL(texture, "texture is null");
    int h;
    int result = SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &h);
    if(result < 0){
      ostringstream error;
      error << "Unable to get texture height" << endl << SDL_GetError();
      logger->log(error.str(), Logger::CERROR);
      throw CapEngineException(error.str());
    }
    return h;
}

//! close a texture openned by the VideoManager
/*!
  \param texture
  \li the texture to close
 */
void VideoManager::closeTexture(Texture* texture) const{
  SDL_DestroyTexture(texture);
}

//! Set the color key for the image
/*! by default it is 0x00ffff
 */
void VideoManager::setColorKey(Surface* surface) const{
  CAP_THROW_NULL(surface, "Cannot set color key on a null surface");
  
  if(SDL_SetColorKey(surface, SDL_TRUE | SDL_RLEACCEL, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF)) == -1){
    throw CapEngineException("Error setting the color key of the surface");
  }
}

Texture* VideoManager::createTexture(int width, int height){
  SDL_Texture* texture = SDL_CreateTexture(m_pRenderer
					   ,SDL_PIXELFORMAT_ARGB8888
					   ,SDL_TEXTUREACCESS_STREAMING
					   , width, height);
  if (texture == nullptr){
    ostringstream error;
    error << "Error creating texture" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR);
    throw CapEngineException(error.str());
  }

  return texture;
}

 void VideoManager::callReshapeFunc(int w, int h){
   if(reshapeFunc == nullptr){
     throw CapEngineException("No reshape function set"); 
   }
   if(currentScreenConfig.opengl){
     reshapeFunc(w, h);
  }
}

void VideoManager::displayFPS(bool on, const string& ttfFontPath, Uint8 r, Uint8 g, Uint8 b){
  showFPS = on;
  this->ttfFontPath = ttfFontPath;
  this->fpsColourR = r;
  this->fpsColourG = g;
  this->fpsColourB = b;
}

Surface* VideoManager::loadSurface(string filePath) const{
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted=IMG_Init(flags);

  if( (initted & flags) != flags) {
    ostringstream error_message;
    error_message <<"could not init SDL_Image" << endl;
    error_message<<"Reason: " << IMG_GetError() << endl;
    throw CapEngineException(error_message.str());
  }


  SDL_Surface* tempSurface = NULL;
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
  
  return tempSurface;
  
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
    
  surface = SDL_CreateRGBSurface(0, width, height, 32,
				 rmask, gmask, bmask, amask);
  if(surface == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }

  setColorKey(surface);

  ostringstream logString;
  logString << "Created new surface with dimensions " << width << "x" << height;
  logger->log(logString.str(), Logger::CDEBUG);
  
  return surface;
}

void VideoManager::closeSurface(Surface* surface) const{
  SDL_FreeSurface(surface);
}

void VideoManager::blitSurface(Surface* sourceSurface, int srcX, int srcY, int sourceWidth, int sourceHeight, Surface* destSurface, int x, int y){
  SDL_Rect srcLocation;
  srcLocation.x = srcX;
  srcLocation.y = srcY;
  srcLocation.w = sourceWidth;
  srcLocation.h = sourceHeight;

  SDL_Rect dstLocation;
  dstLocation.x = x;
  dstLocation.y = y;

  SDL_BlitSurface(sourceSurface, &srcLocation, destSurface, &dstLocation);
}

real VideoManager::getSurfaceWidth(const Surface* surface) const{
  CAP_THROW_NULL(surface, "surface passed to getSurfaceWidth is null");
  return surface->w;
}

real VideoManager::getSurfaceHeight(const Surface* surface) const{
    CAP_THROW_NULL(surface, "surface passed to getSurfaceHeight is null");
    return surface->h;
}

void VideoManager::loadControllerMaps(){
  // generated for wii u pro controller using antimicro
  const char mapping[] = "050000007e0500003003000001000000,Nintendo Wii Remote Pro Controller,platform:Linux,a:b0,b:b0,x:b2,y:b3,back:b8,start:b9,guide:b10,leftshoulder:b4,rightshoulder:b5,leftstick:b11,rightstick:b12,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7,dpup:b13,dpleft:b15,dpdown:b14,dpright:b16,";
  int result = SDL_GameControllerAddMapping(mapping);
  if (result != 0){
    ostringstream error;
    error << "Unable to add controller mapping: " << SDL_GetError();
    logger->log(error.str(), Logger::CWARNING);
  }
}

void VideoManager::loadControllerMapFromFile(std::string path){
  int result = SDL_GameControllerAddMappingsFromFile(path.c_str());
  if (result != 0){
    ostringstream error;
    error << "Error adding controller mappings from file " << path << ": " << SDL_GetError();
    logger->log(error.str(), Logger::CWARNING);
  }
}
