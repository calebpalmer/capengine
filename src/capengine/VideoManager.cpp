//@brief - implementation file for VideoManager class
//@author - Caleb Palmer

#include "VideoManager.h"

#include <SDL2/SDL_image.h>
#include <SDL_blendmode.h>
#include <SDL_pixels.h>

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <boost/throw_exception.hpp>
#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "CapEngineException.h"
#include "EventDispatcher.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "captypes.h"
#include "collision.h"
#include "defer.h"
#include "locator.h"
#include "logger.h"
#include "logging.h"
#include "scopeguard.h"

using namespace std;

namespace CapEngine {

Window::Window()
{
}

Window::Window(SDL_Window* pWindow, SDL_Renderer* pRenderer, Viewport viewport, WindowParams windowParams)
    : m_window(pWindow), m_renderer(pRenderer), m_viewport(viewport), m_windowParams(windowParams)
{
}

bool VideoManager::instantiated = false;

VideoManager::VideoManager()
    : up_fontManager(new FontManager()),
      logger(nullptr),
      m_window(getNullWindowPtr()),
      m_renderer(getNullRendererPtr()),
      initialized(false),
      m_transformationMatrix(Matrix::createIdentityMatrix())
{
    assert(instantiated == false);
    instantiated = true;
    logger.reset(new Logger());
}

VideoManager::VideoManager(Logger* loggerIn)
    : up_fontManager(new FontManager()),
      logger(loggerIn),
      m_window(getNullWindowPtr()),
      m_renderer(getNullRendererPtr()),
      initialized(false),
      m_transformationMatrix(Matrix::createIdentityMatrix())
{
    assert(instantiated == false);
    instantiated = true;
    initialized = false;
}

/**
 Unitializes the system.
 Returns the id of the main window
*/
Uint32 VideoManager::initSystem(WindowParams windowParams, bool noWindow)
{
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
            m_window = createWindow(windowParams);
            m_renderer = createRenderer(m_window.get(), windowParams);

            windowID = SDL_GetWindowID(m_window.get());

            // Create viewpor
            Viewport viewport = {0, 0, windowParams.width, windowParams.height};
            Window window = {m_window.get(), m_renderer.get(), viewport, windowParams};

            m_windows[windowID] = window;
            m_windowNamesToIds[mainWindowName] = windowID;
        }
    }

    // load controller maps
    // loadControllerMaps();

    initialized = true;
    return windowID;
}

TexturePtr VideoManager::createTextureFromSurfacePtr(Surface* surface, bool freeSurface)
{
    auto windowId = getWindowId(mainWindowName);
    return createTextureFromSurfacePtr(windowId, surface, freeSurface);
}

TexturePtr VideoManager::createTextureFromSurfacePtr(Uint32 windowId, Surface* surface, bool freeSurface)
{
    Texture* texture = createTextureFromSurface(windowId, surface, freeSurface);
    return TexturePtr(texture, SDL_DestroyTexture);
}

/**
 * \brief Creates a copy of an existing texture.
 * \param sourceTexture The texture to copy from.
 * \return A new TexturePtr containing a copy of the source texture.
 */
TexturePtr VideoManager::copyTexture(Texture* sourceTexture)
{
    if (sourceTexture == nullptr) {
        CAP_THROW(CapEngineException{"Cannot copy null texture"});
    }

    // Get source texture dimensions and format
    int width = 0;
    int height = 0;
    Uint32 format = 0;
    int access;
    if (SDL_QueryTexture(sourceTexture, &format, &access, &width, &height) != 0) {
        std::ostringstream error;
        error << "Error querying source texture: " << SDL_GetError();
        CAP_THROW(CapEngineException{error.str()});
    }

    // Create destination texture with same dimensions and format
    SDL_Renderer* renderer = m_renderer.get();
    SDL_Texture* destTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
    if (destTexture == nullptr) {
        std::ostringstream error;
        error << "Error creating destination texture: " << SDL_GetError();
        CAP_THROW(CapEngineException{error.str()});
    }

    // Set destination texture as render target
    SDL_Texture* originalTarget = SDL_GetRenderTarget(renderer);
    Defer deferSetRenderTarget([renderer, originalTarget]() { SDL_SetRenderTarget(renderer, originalTarget); });

    if (SDL_SetRenderTarget(renderer, destTexture) != 0) {
        SDL_DestroyTexture(destTexture);
        std::ostringstream error;
        error << "Error setting render target: " << SDL_GetError();
        CAP_THROW(CapEngineException{error.str()});
    }

    // Copy source texture to destination
    if (SDL_RenderCopy(renderer, sourceTexture, nullptr, nullptr) != 0) {
        SDL_SetRenderTarget(renderer, originalTarget);
        SDL_DestroyTexture(destTexture);
        std::ostringstream error;
        error << "Error copying texture: " << SDL_GetError();
        CAP_THROW(CapEngineException{error.str()});
    }

    return TexturePtr(destTexture, SDL_DestroyTexture);
}

Texture* VideoManager::createTextureFromSurface(Surface* surface, bool freeSurface)
{
    auto windowId = getWindowId(mainWindowName);
    return createTextureFromSurface(windowId, surface, freeSurface);
}

Texture* VideoManager::createTextureFromSurface(Uint32 windowId, Surface* surface, bool freeSurface)
{
    // Create Texture from Surface
    auto window = getWindow(windowId);
    auto pRenderer = window.m_renderer;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);
    if (texture == nullptr) {
        ostringstream errorMsg;
        errorMsg << "Unable to load texture from surface " << " - " << SDL_GetError();
        throw CapEngineException(errorMsg.str());
    }

    if (freeSurface) {
        this->closeSurface(surface);
    }
    return texture;
}

TexturePtr VideoManager::loadImagePtr(std::string const& in_filePath) const
{
    Texture* texture = loadImage(in_filePath);
    return TexturePtr(texture, SDL_DestroyTexture);
}

std::shared_ptr<Texture> VideoManager::loadSharedImage(std::string const& in_filePath) const
{
    Texture* texture = loadImage(in_filePath);
    return std::shared_ptr<Texture>(texture, SDL_DestroyTexture);
}

Texture* VideoManager::loadImage(string filePath) const
{
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
    SDL_Surface* tempSurface = NULL;
    tempSurface = IMG_Load(filePath.c_str());
    if (tempSurface == nullptr) {
        ostringstream errorMsg;
        errorMsg << "Unable to load surface " << filePath << " - " << SDL_GetError();
        throw CapEngineException(errorMsg.str());
    }
    ostringstream logString;
    logString << "Loaded surface from file " << filePath;
    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << logString.str();

    setColorKey(tempSurface);

    // Create Texture from Surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.get(), tempSurface);
    if (texture == nullptr) {
        ostringstream errorMsg;
        errorMsg << "Unable to load texture from file " << filePath << " - " << SDL_GetError();
        throw CapEngineException(errorMsg.str());
    }

    SDL_FreeSurface(tempSurface);
    return texture;
}

void VideoManager::drawTexture(Uint32 windowID, Rect dstRect, Texture* texture, Rect* srcRect, bool applyTransform)
{
    auto window = getWindow(windowID);
    auto pRenderer = window.m_renderer;

    Viewport viewport = getViewport(windowID);

    // Transform the dstRect
    if (applyTransform) {
        dstRect = viewport.transformRect(dstRect);
    }

    auto [w, h] = getWindowLogicalResolution(windowID);
    Rect windowRect = {0, 0, w, h};

    // only draw things that are in the window
    if (detectMBRCollision(dstRect, windowRect) != COLLISION_NONE) {
        int result = SDL_RenderCopy(pRenderer, texture, srcRect, &dstRect);
        if (result != 0) {
            logger->log("Unable to render texture", Logger::CERROR, __FILE__, __LINE__);
        }
    }
}

void VideoManager::drawTexture(Uint32 windowID, Texture* texture, Rect* srcRect, Rect* dstRect,
                               std::optional<double> rotationDegrees, SDL_RendererFlip flip, bool applyTransform)
{
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

    auto [w, h] = getWindowResolution(windowID);
    Rect windowRect = {0, 0, w, h};

    // only draw things that are in the window
    if (!dstRect || detectMBRCollision(*dstRect, windowRect) != COLLISION_NONE) {
        if (rotationDegrees) {
            const SDL_Point* center = nullptr;
            SDL_RenderCopyEx(pRenderer, texture, srcRect, dstRect, *rotationDegrees, center, flip);
        }
        else {
            SDL_RenderCopyEx(pRenderer, texture, srcRect, dstRect, 0, nullptr, flip);
        }
    }
}

void VideoManager::drawTexture(Texture* in_dstTexture, Texture* in_srcTexture, Rect& in_dstRect, Rect& in_srcRect)
{
    // set the rendering target to the current texture to have everything
    // rendered to m_texture
    SDL_Renderer* renderer = this->getRenderer();

    Texture* oldTarget = SDL_GetRenderTarget(renderer);
    auto result = SDL_SetRenderTarget(renderer, in_dstTexture);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    Defer deferSetRenderTarget([renderer, oldTarget]() { SDL_SetRenderTarget(renderer, oldTarget); });

    result = SDL_SetTextureBlendMode(in_dstTexture, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    SDL_RenderCopy(renderer, in_srcTexture, &in_srcRect, &in_dstRect);
}

//! Sets the clip rect for a window
/**
\param windowID - The window to set the clip rect on. \param - The retanble to use for clipping. Unsets clip rect if
nullptr;
*/
void VideoManager::setClipRect(Uint32 windowId, SDL_Rect const* clipRect)
{
    Window window = this->getWindow(windowId);
    assert(window.m_renderer != nullptr);

    int rcode = SDL_RenderSetClipRect(window.m_renderer, clipRect);
    if (rcode != 0) {
        assert(logger != nullptr);
        CAP_LOG_SDL_ERROR(logger, Logger::CWARNING);
    }
}

void VideoManager::shutdown()
{
    for (auto& i : m_windows) {
        auto pWindow = i.second.m_window;
        auto pRenderer = i.second.m_renderer;

        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
    }

    SDL_Quit();
    initialized = false;
    instantiated = false;
}

void VideoManager::clearScreen(Uint32 windowID)
{
    auto window = getWindow(windowID);
    auto pRenderer = window.m_renderer;

    SDL_SetRenderDrawColor(pRenderer, m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_g,
                           m_backgroundColour.m_b);

    // Clear Screen
    SDL_RenderClear(pRenderer);
}

void VideoManager::drawScreen(Uint32 windowID)
{
    auto window = getWindow(windowID);
    auto pRenderer = window.m_renderer;

    // Render FPS if turned on
    if (showFPS) {
        string sFPS = to_string(fps);
        int fontSize = 14;
        SurfacePtr fpsSurface =
            up_fontManager->getTextSurface(ttfFontPath, sFPS, fontSize, fpsColourR, fpsColourG, fpsColourB);
        Texture* fpsTexture = SDL_CreateTextureFromSurface(pRenderer, fpsSurface.get());

        int x = 15;
        int y = 15;
        auto textureWidth = this->getTextureWidth(fpsTexture);
        auto textureHeight = this->getTextureHeight(fpsTexture);

        // drawTexture(windowID, x, y, fpsTexture, nullptr, false);

        // draw fpsTexture to the screen at x, y
        drawTexture(windowID, Rect{x, y, x + static_cast<int>(textureWidth), y + static_cast<int>(textureHeight)},
                    fpsTexture, nullptr, false);

        this->closeTexture(fpsTexture);
    }

    // draw the screen
    if (window.m_windowParams.opengl) {
        SDL_GL_SwapWindow(m_window.get());
    }
    else {
        SDL_RenderPresent(pRenderer);
    }

    // Update fps variables
    Time currentTime;
    double elapsedTime = currentTime.subtractTime(&lastRenderTime);
    lastRenderTime = currentTime;
    fps = 1 / (elapsedTime * 0.001);
}

std::pair<int, int> VideoManager::getWindowResolution(Uint32 windowID)
{
    const Window window = getWindow(windowID);
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window.m_window, &width, &height);

    return std::make_pair(width, height);
}

//! Gets the logical size of a window renderer.
/**
\param in_windowID
 The id of the window.
\return
 A pair of ints containing the width and height respectively.
*/
std::pair<int, int> VideoManager::getWindowLogicalResolution(uint32_t in_windowID)
{
    const Window window = getWindow(in_windowID);
    assert(window.m_renderer != nullptr);

    int width = 0;
    int height = 0;
    SDL_RenderGetLogicalSize(window.m_renderer, &width, &height);

    return std::make_pair(width, height);
}

void VideoManager::setWindowLogicalResolution(uint32_t in_windowID, int in_width, int in_height)
{
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
double VideoManager::getTextureWidth(Texture* texture) const
{
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
double VideoManager::getTextureHeight(Texture* texture) const
{
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

void VideoManager::getTextureDims(Texture* texture, int* x, int* y) const
{
    CAP_THROW_NULL(x, "x is null");
    CAP_THROW_NULL(y, "y is null");

    auto [width, height] = this->getTextureDims(texture);
    *x = width;
    *y = height;
}

std::pair<int, int> VideoManager::getTextureDims(Texture* texture) const
{
    CAP_THROW_NULL(texture, "texture is null");

    int x = 0;
    int y = 0;
    int result = SDL_QueryTexture(texture, nullptr, nullptr, &x, &y);
    if (result < 0) {
        ostringstream error;
        error << "Unable to get texture height" << endl << SDL_GetError();
        logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
        throw CapEngineException(error.str());
    }

    return std::make_pair(x, y);
}

//! close a texture openned by the VideoManager
/*!
\param texture
\li the texture to close
*/
void VideoManager::closeTexture(Texture* texture) const
{
    SDL_DestroyTexture(texture);
}

//! Set the color key for the image
/*! by default it is 0x00ffff
 */
void VideoManager::setColorKey(Surface* surface) const
{
    CAP_THROW_NULL(surface, "Cannot set color key on a null surface");

    if (SDL_SetColorKey(surface, SDL_TRUE | SDL_RLEACCEL, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF)) == -1) {
        throw CapEngineException("Error setting the color key of the surface");
    }
}

TexturePtr VideoManager::createTexturePtr(int width, int height, Colour fillColour)
{
    return TexturePtr(createTexture(width, height, fillColour), SDL_DestroyTexture);
}

Texture* VideoManager::createTexture(int width, int height, Colour fillColour)
{
    SDL_Renderer* renderer = m_renderer.get();
    SDL_Texture* texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (texture == nullptr) {
        std::cout << "Dims: " << width << " x " << height << "\n";
        ostringstream error;
        error << "Error creating texture" << endl << SDL_GetError();
        logger->log(error.str(), Logger::CERROR, __FILE__, __LINE__);
        throw CapEngineException(error.str());
    }

    // write out the fill colour
    Texture* oldTarget = SDL_GetRenderTarget(renderer);
    auto result = SDL_SetRenderTarget(renderer, texture);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    Defer deferSetRenderTarget([renderer, oldTarget]() { SDL_SetRenderTarget(renderer, oldTarget); });

    result = SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    SDL_RenderClear(renderer);

    auto windowId = m_windowNamesToIds.at(mainWindowName);
    this->drawFillRect(windowId, Rect{0, 0, width, height}, fillColour);

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

void VideoManager::callReshapeFunc(int w, int h)
{
    if (reshapeFunc == nullptr) {
        throw CapEngineException("No reshape function set");
    }
    if (currentWindowParams.opengl) {
        reshapeFunc(w, h);
    }
}

void VideoManager::displayFPS(bool on, const string& ttfFontPath, Uint8 r, Uint8 g, Uint8 b)
{
    showFPS = on;
    this->ttfFontPath = ttfFontPath;
    this->fpsColourR = r;
    this->fpsColourG = g;
    this->fpsColourB = b;
}

SurfacePtr VideoManager::loadSurfacePtr(std::string const& in_filePath) const
{
    Surface* surface = loadSurface(in_filePath);
    SurfacePtr pSurface(surface, SDL_FreeSurface);
    return pSurface;
}

Surface* VideoManager::loadSurface(string filePath) const
{
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) {
        ostringstream error_message;
        error_message << "could not init SDL_Image" << endl;
        error_message << "Reason: " << IMG_GetError() << endl;
        throw CapEngineException(error_message.str());
    }

    SDL_Surface* tempSurface = NULL;
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
    Surface* surface = createSurface(in_width, in_height);
    CAP_THROW_NULL(surface, "Error creating surface");
    return {surface, SDL_FreeSurface};
}

SurfacePtr VideoManager::createSurfaceFromTexture(Texture* in_texture)
{
    CAP_THROW_NULL(in_texture, "texture is null");
    double width = this->getTextureWidth(in_texture);
    double height = this->getTextureHeight(in_texture);

    SurfacePtr surface = this->createSurfacePtr(width, height);

    SDL_Renderer* renderer = this->getRenderer();
    Texture* oldTarget = SDL_GetRenderTarget(renderer);
    int result = SDL_SetRenderTarget(renderer, in_texture);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    Defer defer{[renderer, oldTarget]() { SDL_SetRenderTarget(renderer, oldTarget); }};

    if (SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch) != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException{"Error reading pixels."});
    }

    return surface;
}

Surface* VideoManager::createSurface(int width, int height)
{
    //     // code taken from http://www.libsdl.org/docs/html/sdlcreatergbsurface.html
    //     /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
    //      as expected by OpenGL for textures */
    //     SDL_Surface* surface;
    //     Uint32 rmask, gmask, bmask, amask;

    //     /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    //      on the endianness (byte order) of the machine */
    // #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    //     rmask = 0xff000000;
    //     gmask = 0x00ff0000;
    //     bmask = 0x0000ff00;
    //     amask = 0x000000ff;
    // #else
    //     rmask = 0x000000ff;
    //     gmask = 0x0000ff00;
    //     bmask = 0x00ff0000;
    //     amask = 0xff000000;
    // #endif

    //     surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    //     if (surface == nullptr) {
    //         fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    //         exit(1);
    //     }

    // Create a surface to hold the pixels
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        CAP_THROW_SDL_ERROR();
    }

    BOOST_LOG_SEV(log, boost::log::trivial::severity_level::debug)
        << "Created new surface with dimensions " << width << "x" << height;

    memset(surface->pixels, 0x00, surface->pitch * surface->h);

    return surface;
}

void VideoManager::closeSurface(Surface* surface) const
{
    SDL_FreeSurface(surface);
}

void VideoManager::saveSurface(SDL_Surface* surface, const std::string& filePath)
{
    if (SDL_SaveBMP(surface, filePath.c_str()) != 0) {
        std::stringstream msg;
        msg << SDL_GetError();
        BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
    }
}

void VideoManager::saveTexture(Texture* texture, std::string const& filePath)
{
    CAP_THROW_NULL(texture, "texture is null");
    SurfacePtr surface = this->createSurfaceFromTexture(texture);
    if (!surface) {
        BOOST_THROW_EXCEPTION(CapEngineException{"Failed to create surface from TiledMap texture"});
    }

    this->saveSurface(surface.get(), filePath);
}

void VideoManager::blitSurface(Surface* sourceSurface, int srcX, int srcY, int sourceWidth, int sourceHeight,
                               Surface* destSurface, int x, int y)
{
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

double VideoManager::getSurfaceWidth(const Surface* surface) const
{
    CAP_THROW_NULL(surface, "surface passed to getSurfaceWidth is null");
    return surface->w;
}

double VideoManager::getSurfaceHeight(const Surface* surface) const
{
    CAP_THROW_NULL(surface, "surface passed to getSurfaceHeight is null");
    return surface->h;
}

void VideoManager::loadControllerMaps()
{
    // generated for wii u pro controller using antimicro
    const char mapping[] =
        "050000007e0500003003000001000000,Nintendo Wii Remote Pro "
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

void VideoManager::loadControllerMapFromFile(std::string path)
{
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
void VideoManager::drawLine(Uint32 windowID, int x1, int y1, int x2, int y2, const Colour& strokeColour)
{
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

void VideoManager::drawFillRect(Texture* in_texture, Rect rect, Colour fillColour)
{
    CAP_THROW_ASSERT(in_texture != nullptr, "Texture is null");
    uint32_t windowId = m_windowNamesToIds.at(mainWindowName);
    SDL_Renderer* renderer = m_windows.at(windowId).m_renderer;
    assert(renderer != nullptr);

    Texture* oldTarget = SDL_GetRenderTarget(renderer);
    assert(in_texture != nullptr);
    auto result = SDL_SetRenderTarget(renderer, in_texture);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    Defer deferSetRenderTarget([renderer, oldTarget]() { SDL_SetRenderTarget(renderer, oldTarget); });

    result = SDL_SetTextureBlendMode(in_texture, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (result != 0) {
        BOOST_THROW_EXCEPTION(CapEngineException(SDL_GetError()));
    }

    this->drawFillRect(windowId, rect, fillColour);
}

void VideoManager::drawFillRect(Uint32 windowID, Rect rect, Colour fillColour)
{
    auto window = getWindow(windowID);
    auto pRenderer = window.m_renderer;
    assert(pRenderer != nullptr);

    Viewport viewport = getViewport(windowID);

    // Transform the dstRect
    Rect newDstRect = viewport.transformRect(rect);

    auto [w, h] = getWindowResolution(windowID);
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

void VideoManager::drawRect(Uint32 windowID, Rect rect, Colour fillColour)
{
    auto window = getWindow(windowID);
    auto pRenderer = window.m_renderer;
    Viewport viewport = getViewport(windowID);

    // Transform the dstRect
    Rect newDstRect = viewport.transformRect(rect);

    auto [w, h] = getWindowResolution(windowID);
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

int VideoManager::toScreenCoord(const Surface* surface, int y) const
{
    CAP_THROW_NULL(surface, "surface is null");
    return surface->h - 1 - y;
}

int VideoManager::fromScreenCoord(const Surface* surface, int y) const
{
    CAP_THROW_NULL(surface, "surface is null");
    return surface->h - 1 - y;
}

void VideoManager::setBackgroundColour(Colour colour)
{
    m_backgroundColour = colour;
}

WindowPtr VideoManager::createWindow(WindowParams windowParams)
{
    Uint32 flags = 0;
    SDL_Window* pWindow = nullptr;

    if (windowParams.fullScreen) {
        // full screen
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
        pWindow = SDL_CreateWindow(windowParams.windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0,
                                   0, flags);
    }
    else {
        // windowed
        if (!windowParams.bordered) {
            flags |= SDL_WINDOW_BORDERLESS;
        }

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

    return WindowPtr(pWindow, &SDL_DestroyWindow);
}

RendererPtr VideoManager::createRenderer(SDL_Window* pWindow, WindowParams windowParams)
{
    SDL_Renderer* pRenderer = nullptr;
    // Now create the 2d Renderer if OpenGL is not being used
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (pRenderer == nullptr) {
        ostringstream errorStream;
        errorStream << "Error creating renderer:  " << SDL_GetError();
        std::cerr << errorStream.str() << std::endl;

        throw CapEngineException(errorStream.str());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
                "linear");  // make the scaled rendering look smoother.

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
                "linear");  // make the scaled rendering look smoother.
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

    return RendererPtr(pRenderer, &SDL_DestroyRenderer);
}

/**
 Creates a new window and returns it's id.
*/
unsigned int VideoManager::createNewWindow(WindowParams windowParams)
{
    WindowPtr windowPtr = createWindow(windowParams);
    RendererPtr rendererPtr = createRenderer(windowPtr.get(), windowParams);
    Viewport viewport(0.0, 0.0, windowParams.width, windowParams.height);

    Window window = {windowPtr.get(), rendererPtr.get(), viewport, windowParams};
    Uint32 id = SDL_GetWindowID(windowPtr.get());
    m_windows[id] = window;
    m_windowNamesToIds[windowParams.name] = id;

    return id;
}

/**
 Closes the window (and renderer) identied by the given windowID
*/
void VideoManager::closeWindow(Uint32 windowID)
{
    try {
        auto window = m_windows.find(windowID);
        if (window != m_windows.end()) {
            SDL_DestroyRenderer(window->second.m_renderer);
            SDL_DestroyWindow(window->second.m_window);
        }
    }
    catch (...) {
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
void VideoManager::setWindowFullScreen(Uint32 windowId, bool fullScreen)
{
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
Window VideoManager::getWindow(Uint32 windowID)
{
    auto window = m_windows.find(windowID);
    if (window == m_windows.end()) {
        ostringstream exceptionDetails;
        exceptionDetails << "Window " << windowID << " not found.";
        throw CapEngineException(exceptionDetails.str());
    }

    return window->second;
}

Uint32 VideoManager::getWindowId(const std::string& windowName) const
{
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
bool VideoManager::isValidWindowId(Uint32 windowId) const
{
    try {
        const_cast<VideoManager*>(this)->getWindow(windowId);
        return true;
    }
    catch (...) {
        return false;
    }
}

/**
 Clears all opened windows.
*/
void VideoManager::clearAll()
{
    for (auto& i : m_windows) {
        auto id = i.first;
        clearScreen(id);
    }
}

/**
 Draws all open windows.
*/
void VideoManager::drawAll()
{
    for (auto& i : m_windows) {
        auto id = i.first;
        drawScreen(id);
    }
}

/**
 Returns a list of ids of open windows
*/
std::vector<Uint32> VideoManager::getWindows() const
{
    std::vector<Uint32> ids;
    for (auto& i : m_windows) {
        ids.push_back(i.first);
    }
    return ids;
}

/**
 Accessor function for FPS metric
*/
int VideoManager::getFPS() const
{
    return fps;
}

/**
 Set Window to full screen or windowed
*/
void VideoManager::setFullscreen(Uint32 windowID, ScreenMode screenMode)
{
    SDL_Window* window = SDL_GetWindowFromID(windowID);
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

void VideoManager::setViewport(Uint32 windowId, Viewport viewport)
{
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

Viewport VideoManager::getViewport(Uint32 windowId) const
{
    // does window exist
    auto windowTuple = m_windows.find(windowId);
    if (windowTuple == m_windows.end()) {
        std::ostringstream errorStream;
        errorStream << "WindowID " << windowId << " does not exist";
        throw CapEngineException(errorStream.str());
    }

    return windowTuple->second.m_viewport;
}

SDL_Renderer* VideoManager::getRenderer()
{
    if (m_renderer == nullptr) {
        BOOST_THROW_EXCEPTION(CapEngineException("Renderer not initialized.  Call init()"));
    }
    return m_renderer.get();
}

TexturePtr textureToTexturePtr(Texture* texture)
{
    return std::move(TexturePtr(texture, SDL_DestroyTexture));
}

void VideoManager::replaceColour(Surface* in_surface, Colour in_oldColour, Colour in_newColour)
{
    CAP_THROW_NULL(in_surface, "Surface is null");

    // SDL_MapRGBA will handle the pixel format conversion of the colour to the pixel format of the surface
    Uint32 oldColour =
        SDL_MapRGBA(in_surface->format, in_oldColour.m_r, in_oldColour.m_g, in_oldColour.m_b, in_oldColour.m_a);
    Uint32 newColour =
        SDL_MapRGBA(in_surface->format, in_newColour.m_r, in_newColour.m_g, in_newColour.m_b, in_newColour.m_a);

    // Lock the surface and iterate through pixels
    SDL_LockSurface(in_surface);
    auto* pixels = reinterpret_cast<Uint32*>(in_surface->pixels);
    for (int i = 0; i < in_surface->w * in_surface->h; i++) {
        if (pixels[i] == oldColour) {
            pixels[i] = newColour;
        }
    }
    SDL_UnlockSurface(in_surface);
}
}  // namespace CapEngine
