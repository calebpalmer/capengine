//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include "VideoManager.h"

#include "CapEngineException.h"
#include "EventDispatcher.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "collision.h"
#include "logger.h"
#include "scopeguard.h"

#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

#include <SDL2/SDL_image.h>

using namespace std;

namespace CapEngine
{

Window::Window() : m_window(nullptr), m_renderer(nullptr) {}

Window::Window(SDL_Window *pWindow, SDL_Renderer *pRenderer, Viewport viewport, WindowParams windowParams)
    : m_window(pWindow), m_renderer(pRenderer), m_viewport(viewport), m_windowParams(windowParams) {}

bool VideoManager::instantiated = false;

VideoManager::VideoManager()
    : up_fontManager(new FontManager()), logger(nullptr), m_pWindow(nullptr), m_pRenderer(nullptr), initialized(false),
      m_transformationMatrix(Matrix::createIdentityMatrix()) {
  assert(instantiated == false);
  instantiated = true;
  logger.reset(new Logger());
}

VideoManager::VideoManager(Logger *loggerIn)
    : up_fontManager(new FontManager()), m_pWindow(nullptr), m_pRenderer(nullptr), logger(loggerIn), initialized(false),
      m_transformationMatrix(Matrix::createIdentityMatrix()) {
  assert(instantiated == false);
  instantiated = true;
  initialized = false;
}

/**
 Unitializes the system.
 Returns the id of the main window
*/
Uint32 VideoManager::initSystem(WindowParams windowParams, bool noWindow) {
  Uint32 windowID = -1;
  currentWindowParams = windowParams;

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    ostringstream errorMsg;
    errorMsg << "Unable to initialize SDL. Shutting down." << endl;
    logger->log(errorMsg.str(), Logger::CERROR, __FILE__, __LINE__);
    shutdown();
  }

  if (!noWindow) {
    if (windowParams.opengl) {
      throw CapEngineException("OpenGL not implemented");
    }

    else {
      m_pWindow = createWindow(windowParams);
      m_pRenderer = createRenderer(m_pWindow, windowParams);

      windowID = SDL_GetWindowID(m_pWindow);

      // Create viewpor
      Viewport viewport = {0, 0, windowParams.width, windowParams.height};
      Window window = {m_pWindow, m_pRenderer, viewport, windowParams};

      m_windows[windowID] = window;
      m_windowNamesToIds[mainWindowName] = windowID;
    }
  }

  // load controller maps
  // loadControllerMaps();

  initialized = true;
  return windowID;
}

TexturePtr VideoManager::createTextureFromSurfacePtr(Surface *surface, bool freeSurface) {
  auto windowId = getWindowId(mainWindowName);
  return createTextureFromSurfacePtr(windowId, surface, freeSurface);
}

TexturePtr VideoManager::createTextureFromSurfacePtr(Uint32 windowId, Surface *surface, bool freeSurface) {
  Texture *texture = createTextureFromSurface(windowId, surface, freeSurface);
  return std::move(TexturePtr(texture, SDL_DestroyTexture));
}

Texture *VideoManager::createTextureFromSurface(Surface *surface, bool freeSurface) {
  auto windowId = getWindowId(mainWindowName);
  return createTextureFromSurface(windowId, surface, freeSurface);
}

Texture *VideoManager::createTextureFromSurface(Uint32 windowId, Surface *surface, bool freeSurface) {
  // Create Texture from Surface
  auto window = getWindow(windowId);
  auto pRenderer = window.m_renderer;

  SDL_Texture *texture = SDL_CreateTextureFromSurface(pRenderer, surface);
  if (texture == nullptr) {
    ostringstream errorMsg;
    errorMsg << "Unable to load texture from surface "
             << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }

  if (freeSurface) {
    this->closeSurface(surface);
  }
  return texture;
}

TexturePtr VideoManager::loadImagePtr(std::string const &in_filePath) const {
  Texture *texture = loadImage(in_filePath);
  return TexturePtr(texture, SDL_DestroyTexture);
}

std::shared_ptr<Texture> VideoManager::loadSharedImage(std::string const &in_filePath) const {
  Texture *texture = loadImage(in_filePath);
  return std::shared_ptr<Texture>(texture, SDL_DestroyTexture);
}

Texture *VideoManager::loadImage(string filePath) const {
  // Try to initialise SDL_Image
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted = IMG_Init(flags);
  if ((initted & flags) != flags) {
    ostringstream error_message;
    error_message << "could not init SDL_Image" << endl;
    error_message << "Reason: " << IMG_GetError() << endl;
    throw CapEngineException(error_message.str());
  }

  // Load Surface
  SDL_Surface *tempSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());
  if (tempSurface == nullptr) {
    ostringstream errorMsg;
    errorMsg << "Unable to load surface " << filePath << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }
  ostringstream logString;
  logString << "Loaded surface from file " << filePath;
  logger->log(logString.str(), Logger::CDEBUG, __FILE__, __LINE__);

  setColorKey(tempSurface);

  // Create Texture from Surface
  SDL_Texture *texture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
  if (texture == nullptr) {
    ostringstream errorMsg;
    errorMsg << "Unable to load texture from file " << filePath << " - " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }

  SDL_FreeSurface(tempSurface);
  return texture;
}

void VideoManager::drawTexture(Uint32 windowID, int x, int y, Texture *texture, Rect *srcRect, bool applyTransform) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;

  Viewport viewport = getViewport(windowID);

  SDL_Rect dstRect;
  dstRect.x = x;
  dstRect.y = y;
  if (srcRect != nullptr) {
    dstRect.w = srcRect->w;
    dstRect.h = srcRect->h;
  }

  else {
    int result = SDL_QueryTexture(texture, nullptr, nullptr, &(dstRect.w), &(dstRect.h));
    if (result != 0) {
      ostringstream errorMsg;
      errorMsg << "Unable to draw texture:  " << SDL_GetError();
      logger->log(errorMsg.str(), Logger::CERROR, __FILE__, __LINE__);
    }
  }

  // Transform the dstRect
  if (applyTransform)
    dstRect = viewport.transformRect(dstRect);

  int w, h;
  getWindowResolution(windowID, &w, &h);
  Rect windowRect = {0, 0, w, h};

  // only draw things that are in the window
  if (detectMBRCollision(dstRect, windowRect) != COLLISION_NONE) {
    int result = SDL_RenderCopy(pRenderer, texture, srcRect, &dstRect);
    if (result != 0) {
      logger->log("Unable to render texture", Logger::CERROR, __FILE__, __LINE__);
    }
  }
}

void VideoManager::drawTexture(Uint32 windowID, Texture *texture, Rect *srcRect, Rect *dstRect,
                               std::optional<double> rotationDegrees, bool applyTransform) {
  assert(texture != nullptr);

  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;

  Viewport viewport = getViewport(windowID);

  // Transform the dstRect
  if (dstRect) {
    Rect newDstRect = *dstRect;
    if (applyTransform)
      newDstRect = viewport.transformRect(*dstRect);
    *dstRect = newDstRect;
  }

  int w, h;
  getWindowResolution(windowID, &w, &h);
  Rect windowRect = {0, 0, w, h};

  // only draw things that are in the window
  if (!dstRect || detectMBRCollision(*dstRect, windowRect) != COLLISION_NONE) {
    if (rotationDegrees) {
      const SDL_Point *center = nullptr;
      SDL_RenderCopyEx(pRenderer, texture, srcRect, dstRect, *rotationDegrees, center, SDL_FLIP_NONE);
    } else
      SDL_RenderCopy(pRenderer, texture, srcRect, dstRect);
  }
}

//! Sets the clip rect for a window
/**
\param windowID - The window to set the clip rect on.
                                                                                                                                                                                                                                                                  \param - The retanble to use for clipping.
Unsets clip rect if nullptr;
*/
void VideoManager::setClipRect(Uint32 windowId, SDL_Rect const *clipRect) {
  Window window = this->getWindow(windowId);
  assert(window.m_renderer != nullptr);

  int rcode = SDL_RenderSetClipRect(window.m_renderer, clipRect);
  if (rcode != 0) {
    assert(logger != nullptr);
    CAP_LOG_SDL_ERROR(logger, Logger::CWARNING);
  }
}

void VideoManager::shutdown() {
  for (auto &i : m_windows) {
    auto pWindow = i.second.m_window;
    auto pRenderer = i.second.m_renderer;

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
  }

  SDL_Quit();
  initialized = false;
  instantiated = false;
}

void VideoManager::clearScreen(Uint32 windowID) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;

  SDL_SetRenderDrawColor(pRenderer, m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_g,
                         m_backgroundColour.m_b);

  // Clear Screen
  SDL_RenderClear(pRenderer);
}

void VideoManager::drawScreen(Uint32 windowID) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;

  // Render FPS if turned on
  if (showFPS) {
    string sFPS = to_string(fps);
    int fontSize = 14;
    Surface *fpsSurface =
        up_fontManager->getTextSurface(ttfFontPath, sFPS, fontSize, fpsColourR, fpsColourG, fpsColourB);
    Texture *fpsTexture = SDL_CreateTextureFromSurface(pRenderer, fpsSurface);
    SDL_FreeSurface(fpsSurface);

    int x = 15;
    int y = 15;
    drawTexture(windowID, x, y, fpsTexture, nullptr, false);
    this->closeTexture(fpsTexture);
  }

  // draw the screen
  if (window.m_windowParams.opengl) {
    SDL_GL_SwapWindow(m_pWindow);
  } else {
    SDL_RenderPresent(pRenderer);
  }

  // Update fps variables
  Time currentTime;
  double elapsedTime = currentTime.subtractTime(&lastRenderTime);
  lastRenderTime = currentTime;
  fps = 1 / (elapsedTime * 0.001);
}

void VideoManager::getWindowResolution(Uint32 windowID, int *width, int *height)
{
  const Window window = getWindow(windowID);
  SDL_GetWindowSize(window.m_window, width, height);
}

//! Gets the logical size of a window renderer.
/**
\param in_windowID
 The id of the window.
\return
 A pair of ints containing the width and height respectively.
*/
std::pair<int, int> VideoManager::getWindowLogicalResolution(uint32_t in_windowID) {
  const Window window = getWindow(in_windowID);
  assert(window.m_renderer != nullptr);

  int width = 0;
  int height = 0;
  SDL_RenderGetLogicalSize(window.m_renderer, &width, &height);

  return std::make_pair(width, height);
}

void VideoManager::setWindowLogicalResolution(uint32_t in_windowID, int in_width, int in_height) {
  const Window window = getWindow(in_windowID);
  assert(window.m_renderer != nullptr);

  int result = SDL_RenderSetLogicalSize(window.m_renderer, in_width, in_height);
  if (result) {
    std::stringstream error;
    error << "SDL_RenderSetLogicalSize() failed: " << SDL_GetError();
    CAP_THROW(CapEngineException(error.str()));
  }
}

int VideoManager::getWindowWidth(Uint32 windowID)
{
  const Window window = getWindow(windowID);
  int w;
  int h;
  SDL_GetWindowSize(window.m_window, &w, &h);
  return w;
}

int VideoManager::getWindowHeight(Uint32 windowID)
{
  const Window window = getWindow(windowID);
  int w;
  int h;
  SDL_GetWindowSize(window.m_window, &w, &h);
  return h;
}

//! Sets the position of a window
/**
 * \param windowId The window id of the window
 * \param x The x position of the upper left of the window
 * \param y The y position of the upper left of the window
 */
void VideoManager::setWindowPosition(Uint32 windowId, int x, int y)
{
  Window window = getWindow(windowId);
  SDL_SetWindowPosition(window.m_window, x, y);
}

//! Sets the size of a window
/**
 * \param windowId The window id of the window
 * \param width The width of the window
 * \param height The height of the window
 */
void VideoManager::setWindowSize(Uint32 windowId, int width, int height)
{
  Window window = getWindow(windowId);
  SDL_SetWindowSize(window.m_window, width, height);
}

//! get the width of given texture
/*!
\param texture
*/
real VideoManager::getTextureWidth(Texture *texture) const {
  CAP_THROW_NULL(texture, "Texture is null");
  int w;
  int result = SDL_QueryTexture(texture, nullptr, nullptr, &w, nullptr);
  if (result < 0) {
    ostringstream error;
    error << "Unable to get texture width" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
    throw CapEngineException(error.str());
  }
  return w;
}

//! get the height of given texture
/*!
\param texture
*/
real VideoManager::getTextureHeight(Texture *texture) const {
  CAP_THROW_NULL(texture, "texture is null");
  int h;
  int result = SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &h);
  if (result < 0) {
    ostringstream error;
    error << "Unable to get texture height" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
    throw CapEngineException(error.str());
  }
  return h;
}

void VideoManager::getTextureDims(Texture *texture, int *x, int *y) const {
  CAP_THROW_NULL(texture, "texture is null");
  int result = SDL_QueryTexture(texture, nullptr, nullptr, x, y);
  if (result < 0) {
    ostringstream error;
    error << "Unable to get texture height" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
    throw CapEngineException(error.str());
  }
}

//! close a texture openned by the VideoManager
/*!
\param texture
\li the texture to close
*/
void VideoManager::closeTexture(Texture *texture) const
{
  SDL_DestroyTexture(texture);
}

//! Set the color key for the image
/*! by default it is 0x00ffff
 */
void VideoManager::setColorKey(Surface *surface) const {
  CAP_THROW_NULL(surface, "Cannot set color key on a null surface");

  if (SDL_SetColorKey(surface, SDL_TRUE | SDL_RLEACCEL, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF)) == -1) {
    throw CapEngineException("Error setting the color key of the surface");
  }
}

TexturePtr VideoManager::createTexturePtr(int width, int height)
{
  Texture *texture = createTexture(width, height);
  return std::move(TexturePtr(texture, SDL_DestroyTexture));
}

Texture *VideoManager::createTexture(int width, int height) {
  SDL_Texture *texture =
      SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
  if (texture == nullptr) {
    ostringstream error;
    error << "Error creating texture" << endl << SDL_GetError();
    logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
    throw CapEngineException(error.str());
  }

  return texture;
}

//! Sets the reshape function.
/**
\param func
 The reshape function pointer
*/
void VideoManager::setReshapeFunc(void (*func)(int x, int y))
{
  reshapeFunc = func;
}

void VideoManager::callReshapeFunc(int w, int h) {
  if (reshapeFunc == nullptr) {
    throw CapEngineException("No reshape function set");
  }
  if (currentWindowParams.opengl) {
    reshapeFunc(w, h);
  }
}

void VideoManager::displayFPS(bool on, const string &ttfFontPath, Uint8 r, Uint8 g, Uint8 b) {
  showFPS = on;
  this->ttfFontPath = ttfFontPath;
  this->fpsColourR = r;
  this->fpsColourG = g;
  this->fpsColourB = b;
}

SurfacePtr VideoManager::loadSurfacePtr(std::string const &in_filePath) const
{
  Surface *surface = loadSurface(in_filePath);
  SurfacePtr pSurface(surface, SDL_FreeSurface);
  return pSurface;
}

Surface *VideoManager::loadSurface(string filePath) const {
  int flags = IMG_INIT_JPG | IMG_INIT_PNG;
  int initted = IMG_Init(flags);

  if ((initted & flags) != flags) {
    ostringstream error_message;
    error_message << "could not init SDL_Image" << endl;
    error_message << "Reason: " << IMG_GetError() << endl;
    throw CapEngineException(error_message.str());
  }

  SDL_Surface *tempSurface = NULL;
  tempSurface = IMG_Load(filePath.c_str());

  ostringstream logString;
  logString << "Loaded surface from file " << filePath;
  logger->log(logString.str(), Logger::CDEBUG, __FILE__, __LINE__);

  if (tempSurface == NULL) {
    cerr << "Unable to load surface" << endl;
    cerr << "SDL Error: " << SDL_GetError() << endl;
    return tempSurface;
  }

  setColorKey(tempSurface);

  return tempSurface;
}

SurfacePtr VideoManager::createSurfacePtr(int in_width, int in_height)
{
  Surface *surface = createSurface(in_width, in_height);
  return std::move(SurfacePtr(surface, SDL_FreeSurface));
}

Surface *VideoManager::createSurface(int width, int height) {
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

  surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
  if (surface == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }

  // setColorKey(surface);

  ostringstream logString;
  logString << "Created new surface with dimensions " << width << "x" << height;
  logger->log(logString.str(), Logger::CDEBUG, __FILE__, __LINE__);

  memset(surface->pixels, 0x00, surface->pitch * surface->h);

  return surface;
}

void VideoManager::closeSurface(Surface *surface) const
{
  SDL_FreeSurface(surface);
}

void VideoManager::saveSurface(SDL_Surface *surface, const std::string &filePath) {
  if (SDL_SaveBMP(surface, filePath.c_str()) != 0) {
    std::stringstream msg;
    msg << SDL_GetError();
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }
}

void VideoManager::blitSurface(Surface *sourceSurface, int srcX, int srcY, int sourceWidth, int sourceHeight,
                               Surface *destSurface, int x, int y) {
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

real VideoManager::getSurfaceWidth(const Surface *surface) const
{
  CAP_THROW_NULL(surface, "surface passed to getSurfaceWidth is null");
  return surface->w;
}

real VideoManager::getSurfaceHeight(const Surface *surface) const
{
  CAP_THROW_NULL(surface, "surface passed to getSurfaceHeight is null");
  return surface->h;
}

void VideoManager::loadControllerMaps() {
  // generated for wii u pro controller using antimicro
  const char mapping[] = "050000007e0500003003000001000000,Nintendo Wii Remote Pro "
                         "Controller,platform:Linux,a:b0,b:b0,x:b2,y:b3,back:b8,start:b9,guide:"
                         "b10,leftshoulder:b4,rightshoulder:b5,leftstick:b11,rightstick:b12,"
                         "leftx:"
                         "a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7,dpup:"
                         "b13,"
                         "dpleft:b15,dpdown:b14,dpright:b16,";
  int result = SDL_GameControllerAddMapping(mapping);
  if (result != 0) {
    ostringstream error;
    error << "Unable to add controller mapping: " << SDL_GetError();
    logger->log(error.str(), Logger::CWARNING, __FILE__, __LINE__);
  }
}

void VideoManager::loadControllerMapFromFile(std::string path) {
  int result = SDL_GameControllerAddMappingsFromFile(path.c_str());
  if (result != 0) {
    ostringstream error;
    error << "Error adding controller mappings from file " << path << ": " << SDL_GetError();
    logger->log(error.str(), Logger::CWARNING, __FILE__, __LINE__);
  }
}

//! Draws a line.
/**
\param x1
 \li The x coordinate to start at.
\param y1
 \li The y coordinate to start at.
\param x2
 \li The x coordinate to end with.
\param y2
 \li The y coordinate to end with.
\param strokeColour
 \li The colour of the line.
*/
void VideoManager::drawLine(Uint32 windowID, int x1, int y1, int x2, int y2, const Colour &strokeColour) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;
  Viewport viewport = getViewport(windowID);

  // Transform the dstRect
  Point point1{x1, y1};
  point1 = viewport.transform(point1);

  Point point2{x2, y2};
  point2 = viewport.transform(point2);

  // set the colour
  Uint8 r, g, b, a;
  int rcode = SDL_GetRenderDrawColor(pRenderer, &r, &g, &b, &a);
  if (rcode != 0) {
    CAP_LOG_SDL_ERROR(logger, Logger::CWARNING);
    return;
  }

  ScopeGuard resetColour([&]() { SDL_SetRenderDrawColor(pRenderer, r, g, b, a); });

  SDL_SetRenderDrawColor(pRenderer, strokeColour.m_r, strokeColour.m_g, strokeColour.m_b, strokeColour.m_a);
  SDL_RenderDrawLine(pRenderer, point1.x, point1.y, point2.x, point2.y);
}

void VideoManager::drawFillRect(Uint32 windowID, Rect rect, Colour fillColour) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;
  assert(pRenderer != nullptr);

  Viewport viewport = getViewport(windowID);

  // Transform the dstRect
  Rect newDstRect = viewport.transformRect(rect);

  int w, h;
  getWindowResolution(windowID, &w, &h);
  Rect windowRect = {0, 0, w, h};

  // only draw things that are in the window
  if (detectMBRCollision(newDstRect, windowRect) != COLLISION_NONE) {
    // Draw the rect
    SDL_SetRenderDrawColor(pRenderer, fillColour.m_r, fillColour.m_g, fillColour.m_b, fillColour.m_a);
    if (SDL_RenderFillRect(pRenderer, &newDstRect) != 0) {
      string errorMessage(SDL_GetError());
      logger->log(errorMessage, Logger::CWARNING, __FILE__, __LINE__);
    }
  }
}

void VideoManager::drawRect(Uint32 windowID, Rect rect, Colour fillColour) {
  auto window = getWindow(windowID);
  auto pRenderer = window.m_renderer;
  Viewport viewport = getViewport(windowID);

  // Transform the dstRect
  Rect newDstRect = viewport.transformRect(rect);

  int w, h;
  getWindowResolution(windowID, &w, &h);
  Rect windowRect = {0, 0, w, h};

  // only draw things that are in the window
  if (detectMBRCollision(newDstRect, windowRect) != COLLISION_NONE) {
    // render the the rect
    SDL_SetRenderDrawColor(pRenderer, fillColour.m_r, fillColour.m_g, fillColour.m_g, fillColour.m_a);
    if (SDL_RenderDrawRect(pRenderer, &rect) != 0) {
      string errorMessage(SDL_GetError());
      logger->log(errorMessage, Logger::CWARNING, __FILE__, __LINE__);
    }
  }
}

int VideoManager::toScreenCoord(const Surface *surface, int y) const
{
  CAP_THROW_NULL(surface, "surface is null");
  return surface->h - 1 - y;
}

int VideoManager::fromScreenCoord(const Surface *surface, int y) const
{
  CAP_THROW_NULL(surface, "surface is null");
  return surface->h - 1 - y;
}

void VideoManager::setBackgroundColour(Colour colour)
{
  m_backgroundColour = colour;
}

SDL_Window *VideoManager::createWindow(WindowParams windowParams) {
  Uint32 flags = 0;
  SDL_Window *pWindow = nullptr;

  if (windowParams.fullScreen) {
    // full screen
    flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    pWindow = SDL_CreateWindow(windowParams.windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
                               flags);
  } else {
    // windowed
    flags |= SDL_WINDOW_BORDERLESS;

    if (windowParams.resizable)
      flags = flags | SDL_WINDOW_RESIZABLE;

    if (windowParams.maximized)
      flags = flags | SDL_WINDOW_MAXIMIZED;

    pWindow = SDL_CreateWindow(windowParams.windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowParams.width, windowParams.height, flags);
  }

  // check for error
  if (pWindow == nullptr) {
    ostringstream errorStream;
    errorStream << "Error creating window: " << SDL_GetError();
    throw CapEngineException(errorStream.str());
  }

  return pWindow;
}

SDL_Renderer *VideoManager::createRenderer(SDL_Window *pWindow, WindowParams windowParams) {
  SDL_Renderer *pRenderer = nullptr;
  // Now create the 2d Renderer if OpenGL is not being used
  pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
  if (pRenderer == nullptr) {
    ostringstream errorStream;
    errorStream << "Error creating renderer:  " << SDL_GetError();
    std::cerr << errorStream.str() << std::endl;

    throw CapEngineException(errorStream.str());
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
              "linear"); // make the scaled rendering look smoother.

  // if full, set scaling
  // if(windowParams.fullScreen){
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
              "linear"); // make the scaled rendering look smoother.
  int result = SDL_RenderSetLogicalSize(pRenderer, windowParams.width, windowParams.height);
  if (result != 0) {
    ostringstream errorMsg;
    errorMsg << "Unable to set logical render size: " << SDL_GetError();
    logger->log(errorMsg.str(), Logger::CERROR, __FILE__, __LINE__);
    //  }
  }

  SDL_SetRenderDrawColor(pRenderer, m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_g, 255);

  // do alpha blending
  SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);

  return pRenderer;
}

/**
 Creates a new window and returns it's id.
*/
unsigned int VideoManager::createNewWindow(WindowParams windowParams)
{
  SDL_Window *pWindow = createWindow(windowParams);
  SDL_Renderer *pRenderer = createRenderer(pWindow, windowParams);
  Viewport viewport(0.0, 0.0, windowParams.width, windowParams.height);

  Window window = {pWindow, pRenderer, viewport, windowParams};
  Uint32 id = SDL_GetWindowID(pWindow);
  m_windows[id] = window;
  m_windowNamesToIds[windowParams.name] = id;

  return id;
}

/**
 Closes the window (and renderer) identied by the given windowID
*/
void VideoManager::closeWindow(Uint32 windowID) {
  try {
    auto window = m_windows.find(windowID);
    if (window != m_windows.end()) {
      SDL_DestroyRenderer(window->second.m_renderer);
      SDL_DestroyWindow(window->second.m_window);
    }
  } catch (...) {
  }
}

//! Maximize a window
/**
\param windowId
 \li The if of the window to maximize.
*/
void VideoManager::maximizeWindow(Uint32 windowId)
{
  Window window = this->getWindow(windowId);

  assert(window.m_window != nullptr);
  SDL_MaximizeWindow(window.m_window);
}

//! Set the window to fullscreen or not.
/**
\param windowId
 \li The id of the window.
\param fullScreen
 \li flag indicating if window should be full screen or not.
*/
void VideoManager::setWindowFullScreen(Uint32 windowId, bool fullScreen) {
  Window window = this->getWindow(windowId);

  assert(window.m_window != nullptr);
  if (fullScreen)
    SDL_SetWindowFullscreen(window.m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  else
    SDL_SetWindowFullscreen(window.m_window, 0);
}

/**
 Returns the Window for a given window ID
*/
Window VideoManager::getWindow(Uint32 windowID) {
  auto window = m_windows.find(windowID);
  if (window == m_windows.end()) {
    ostringstream exceptionDetails;
    exceptionDetails << "Window " << windowID << " not found.";
    throw CapEngineException(exceptionDetails.str());
  }

  return window->second;
}

Uint32 VideoManager::getWindowId(const std::string &windowName) const {
  auto window = m_windowNamesToIds.find(windowName);
  if (window == m_windowNamesToIds.end()) {
    std::ostringstream msg;
    msg << "Window \"" << windowName << "\" does not exist";
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }

  return window->second;
}

//! Check if window ID is valid
/**
 * \param windowId - The windowId to check
 * \return true if it is valid, false otherwise
 */
bool VideoManager::isValidWindowId(Uint32 windowId) const {
  try {
    const_cast<VideoManager *>(this)->getWindow(windowId);
    return true;
  } catch (...) {
    return false;
  }
}

/**
 Clears all opened windows.
*/
void VideoManager::clearAll() {
  for (auto &i : m_windows) {
    auto id = i.first;
    clearScreen(id);
  }
}

/**
 Draws all open windows.
*/
void VideoManager::drawAll() {
  for (auto &i : m_windows) {
    auto id = i.first;
    drawScreen(id);
  }
}

/**
 Returns a list of ids of open windows
*/
std::vector<Uint32> VideoManager::getWindows() const {
  std::vector<Uint32> ids;
  for (auto &i : m_windows) {
    ids.push_back(i.first);
  }
  return ids;
}

/**
 Accessor function for FPS metric
*/
int VideoManager::getFPS() const { return fps; }

/**
 Set Window to full screen or windowed
*/
void VideoManager::setFullscreen(Uint32 windowID, ScreenMode screenMode) {
  SDL_Window *window = SDL_GetWindowFromID(windowID);
  if (window == nullptr) {
    ostringstream errorMsg;
    errorMsg << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }
  int result = SDL_SetWindowFullscreen(window, screenMode);
  if (result != 0) {
    ostringstream errorMsg;
    errorMsg << "Unable to set window fullscreen: " << SDL_GetError();
    throw CapEngineException(errorMsg.str());
  }
}

void VideoManager::setViewport(Uint32 windowId, Viewport viewport) {
  // does window exist
  auto windowTuple = m_windows.find(windowId);
  if (windowTuple == m_windows.end()) {
    std::ostringstream errorStream;
    errorStream << "WindowID " << windowId << " does not exist";
    throw CapEngineException(errorStream.str());
  }

  // update window
  Window updatedWindow = windowTuple->second;
  updatedWindow.m_viewport = viewport;
  m_windows[windowId] = updatedWindow;
}

Viewport VideoManager::getViewport(Uint32 windowId) const {
  // does window exist
  auto windowTuple = m_windows.find(windowId);
  if (windowTuple == m_windows.end()) {
    std::ostringstream errorStream;
    errorStream << "WindowID " << windowId << " does not exist";
    throw CapEngineException(errorStream.str());
  }

  return windowTuple->second.m_viewport;
}

TexturePtr textureToTexturePtr(Texture *texture)
{
  return std::move(TexturePtr(texture, SDL_DestroyTexture));
}

} // namespace CapEngine
