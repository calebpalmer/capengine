#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

//@brief - header file for VideoManager class
//@author - Caleb Palmer

#include <string>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "captypes.h"
#include "CapEngineException.h"
#include "Time.h"
#include "fontmanager.h"
#include "logger.h"
#include "colour.h"
#include "matrix.h"
#include "viewport.h"

namespace CapEngine {

  enum ScreenMode{
    ScreenMode_FullScreen,
    ScreenMode_Windowed
  };

  struct WindowParams{
    std::string windowName;
    int width;
    int height;
    int pDepth;
    bool fullScreen;
    bool opengl;
    bool resizable;
  };

  struct Window {
    Window();
    Window(SDL_Window* pWindow, SDL_Renderer* pRenderer,
	   Viewport viewport);

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    Viewport m_viewport;
  };

  class FontManager;

  static const WindowParams defaultScreen = {"CapEngine",1280,800,32,false, false,false};


  class VideoManager {
  public:
    VideoManager();    
    VideoManager(Logger* loggerIn);
    Uint32 initSystem(WindowParams windowParams);
    Uint32 createNewWindow(WindowParams windowParams);
    void closeWindow(Uint32 windowID);
    std::vector<Uint32> getWindows() const;
    void shutdown();
    void clearScreen(Uint32 windowID);
    void drawScreen(Uint32 windowID);
    void clearAll();
    void drawAll();
    void getWindowResolution(Uint32 windowID, int* width, int* height);
    int getWindowWidth(Uint32 windowID);
    int getWindowHeight(Uint32 windowID);
    void setViewport();
    void setBackgroundColour(Colour colour);
    int getFPS() const;
    void setFullscreen(Uint32 windowID, ScreenMode screenMode);
    void setViewport(Uint32 windowId, Viewport viewport);
    Viewport getViewport(Uint32 windowId) const;
    
    // Surfaces
    Surface* loadSurface(std::string fileName) const;
    SurfacePtr loadSurfacePtr(std::string const& in_filePath) const;
    Surface* createSurface(int width, int height);
    SurfacePtr createSurfacePtr(int in_width, int in_height);
    void saveSurface(Surface* surface, const std::string& filePath);
    void closeSurface(Surface* surface) const;
    void blitSurface(Surface* sourceSurface, int srcX, int srcY,  int sourceWidth, int sourceHeight, Surface* destSurface, int x, int y);
    real getSurfaceWidth(const Surface* surface) const;
    real getSurfaceHeight(const Surface* surface) const;
    // Textures
    Texture* loadImage(std::string fileName) const;
    TexturePtr loadImagePtr(std::string const& in_filePath) const;
    void closeTexture(Texture* texture) const;
    void drawTexture(Uint32 windowID, int x, int y, Texture* texture, Rect* srcRect=nullptr, bool applyTransform=true);
    void drawTexture(Uint32 windowID, Texture* texture, Rect* srcRect, Rect* dstRect, bool applyTransform=true);
    real getTextureWidth(Texture* texture) const;
    real getTextureHeight(Texture* texture) const;
    void getTextureDims(Texture* texture, int* x, int* y) const;
    Texture* createTexture(int width, int height);
    TexturePtr createTexturePtr(int width, int height);
    Texture* createTextureFromSurface(Surface* surface, bool freeSurface = false); 
    TexturePtr createTextureFromSurfacePtr(Surface* surface, bool freeSurface = false); 

    //opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath="", Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

    // input
    void loadControllerMapFromFile(std::string filePath);
    //Drawing
    void drawRect(Uint32 windowID, Rect rect, Colour fillColour);

    // utility
    int toScreenCoord(Surface* surface, int y);

    bool initialized;

  protected:
    void setColorKey(Surface* surface) const;
    void loadControllerMaps();

  private:
    // private for singleton
    VideoManager(const VideoManager& videoManager);
    VideoManager& operator=(const VideoManager& videoManager);

    SDL_Window* createWindow(WindowParams windowParams);
    SDL_Renderer* createRenderer(SDL_Window* window, WindowParams windowParams);
    Window getWindow(Uint32 windowID);

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    std::map<Uint32, Window> m_windows;

    Matrix m_transformationMatrix;
    
    Logger* logger;
    static bool instantiated; //singleton
    WindowParams currentWindowParams;
    void (*reshapeFunc)(int, int); //for opengl resize functions
    CapEngine::Time lastRenderTime;
    float fps;
    std::unique_ptr<CapEngine::FontManager> up_fontManager;

    bool showFPS;
    std::string ttfFontPath;
    Uint8 fpsColourR;
    Uint8 fpsColourG;
    Uint8 fpsColourB;
    Colour m_backgroundColour;
  };

}

#endif
