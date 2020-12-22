#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

//@brief - header file for VideoManager class
//@author - Caleb Palmer

#include <memory>
#include <string>
#include <vector>

#include "CapEngineException.h"
#include "Time.h"
#include "captypes.h"
#include "colour.h"
#include "fontmanager.h"
#include "logger.h"
#include "matrix.h"
#include "viewport.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <optional>

namespace CapEngine
{

enum ScreenMode { ScreenMode_FullScreen, ScreenMode_Windowed };

struct WindowParams {
  std::string windowName;
  int width = 0;
  int height = 0;
  int pDepth = 0;
  bool fullScreen = false;
  bool opengl = false;
  bool resizable = true;
  bool maximized = false;
  std::string name;
};

struct Window {
  Window();
  Window(SDL_Window *pWindow, SDL_Renderer *pRenderer, Viewport viewport,
         WindowParams windowParams);

  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  Viewport m_viewport;
  WindowParams m_windowParams;
};

class FontManager;

static const WindowParams defaultScreen = {
    "CapEngine", 1280, 800, 32, false, false, true, false, "main"};

// free functions
TexturePtr textureToTexturePtr(Texture *texture);

class VideoManager
{
public:
  VideoManager();
  explicit VideoManager(Logger *loggerIn);
  virtual ~VideoManager() = default;

  virtual Uint32 initSystem(WindowParams windowParams, bool noWindow = false);
  virtual std::vector<Uint32> getWindows() const;
  virtual void shutdown();
  virtual void clearScreen(Uint32 windowID);
  virtual void drawScreen(Uint32 windowID);
  virtual void clearAll();
  virtual void drawAll();

  // Windows
  virtual Uint32 createNewWindow(WindowParams windowParams);
  virtual void closeWindow(Uint32 windowID);
  virtual Uint32 getWindowId(const std::string &windowName) const;
  virtual bool isValidWindowId(Uint32 windowId) const;
  virtual void getWindowResolution(Uint32 windowID, int *width, int *height);
  virtual std::pair<int, int> getWindowLogicalResolution(uint32_t in_windowID);
  virtual void setWindowLogicalResolution(uint32_t in_windowID, int in_width,
                                          int in_height);
  virtual int getWindowWidth(Uint32 windowID);
  virtual int getWindowHeight(Uint32 windowID);
  virtual void setWindowPosition(Uint32 windowId, int x, int y);
  virtual void setWindowSize(Uint32 windowId, int width, int height);
  virtual void maximizeWindow(Uint32 windowId);
  virtual void setWindowFullScreen(Uint32 windowId, bool fullScreen);

  virtual void setBackgroundColour(Colour colour);
  virtual int getFPS() const;
  virtual void setFullscreen(Uint32 windowID, ScreenMode screenMode);
  virtual void setViewport(Uint32 windowId, Viewport viewport);
  virtual Viewport getViewport(Uint32 windowId) const;

  // Surfaces
  virtual Surface *loadSurface(std::string fileName) const;
  virtual SurfacePtr loadSurfacePtr(std::string const &in_filePath) const;
  virtual Surface *createSurface(int width, int height);
  virtual SurfacePtr createSurfacePtr(int in_width, int in_height);
  virtual void saveSurface(Surface *surface, const std::string &filePath);
  virtual void closeSurface(Surface *surface) const;
  virtual void blitSurface(Surface *sourceSurface, int srcX, int srcY,
                           int sourceWidth, int sourceHeight,
                           Surface *destSurface, int x, int y);
  virtual real getSurfaceWidth(const Surface *surface) const;
  virtual real getSurfaceHeight(const Surface *surface) const;
  // Textures
  virtual Texture *loadImage(std::string fileName) const;
  virtual TexturePtr loadImagePtr(std::string const &in_filePath) const;
  virtual void closeTexture(Texture *texture) const;
  virtual void drawTexture(Uint32 windowID, int x, int y, Texture *texture,
                           Rect *srcRect = nullptr, bool applyTransform = true);
  virtual void drawTexture(Uint32 windowID, Texture *texture, Rect *srcRect,
                           Rect *dstRect,
                           std::optional<double> rotationDegrees = std::nullopt,
                           bool applyTransform = true);
  virtual real getTextureWidth(Texture *texture) const;
  virtual real getTextureHeight(Texture *texture) const;
  virtual void getTextureDims(Texture *texture, int *x, int *y) const;
  virtual Texture *createTexture(int width, int height);
  virtual TexturePtr createTexturePtr(int width, int height);
  virtual Texture *createTextureFromSurface(Surface *surface,
                                            bool freeSurface = false);
  virtual Texture *createTextureFromSurface(Uint32 windowID, Surface *surface,
                                            bool freeSurface = false);
  virtual TexturePtr createTextureFromSurfacePtr(Surface *surface,
                                                 bool freeSurface = false);
  virtual TexturePtr createTextureFromSurfacePtr(Uint32 windowId,
                                                 Surface *surface,
                                                 bool freeSurface = false);
  virtual void setClipRect(Uint32 windowId, SDL_Rect const *clipRect);

  // opengl support
  virtual void setReshapeFunc(void (*func)(int x, int y));
  virtual void callReshapeFunc(int w, int h);
  virtual void displayFPS(bool on, const std::string &ttfFontPath = "",
                          Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

  // input
  virtual void loadControllerMapFromFile(std::string filePath);
  // Drawing
  virtual void drawLine(Uint32 windowID, int x1, int y1, int x2, int y2,
                        const Colour &strokeColour);
  virtual void drawFillRect(Uint32 windowID, Rect rect, Colour fillColour);
  virtual void drawRect(Uint32 windowID, Rect rect, Colour fillColour);

  // utility
  virtual int toScreenCoord(const Surface *surface, int y) const;
  virtual int fromScreenCoord(const Surface *surface, int y) const;

  bool initialized = false;
  const std::string mainWindowName = "main";

  static const int kInvalidWindowId = 0; //<! The of an invalid window

protected:
  void setColorKey(Surface *surface) const;
  void loadControllerMaps();

private:
  VideoManager(const VideoManager &videoManager);
  VideoManager &operator=(const VideoManager &videoManager);

  SDL_Window *createWindow(WindowParams windowParams);
  SDL_Renderer *createRenderer(SDL_Window *window, WindowParams windowParams);
  Window getWindow(Uint32 windowID);

  SDL_Window *m_pWindow;
  SDL_Renderer *m_pRenderer;
  std::map<Uint32, Window> m_windows;
  std::map<std::string, Uint32> m_windowNamesToIds;

  Matrix m_transformationMatrix;

  Logger *logger;
  static bool instantiated; // singleton
  WindowParams currentWindowParams;
  void (*reshapeFunc)(int, int); // for opengl resize functions
  CapEngine::Time lastRenderTime;
  float fps;
  std::unique_ptr<CapEngine::FontManager> up_fontManager;

  bool showFPS = false;
  std::string ttfFontPath;
  Uint8 fpsColourR;
  Uint8 fpsColourG;
  Uint8 fpsColourB;
  Colour m_backgroundColour = {0, 0, 0, 255};
};

} // namespace CapEngine

#endif
