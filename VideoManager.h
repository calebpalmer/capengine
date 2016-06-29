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

namespace CapEngine {

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
    Window() {}
    
    Window(SDL_Window* window,
	   SDL_Renderer* renderer)
    : m_window(window),
      m_renderer(renderer) {}

    Window(const Window& oldWindow) {
      m_window = oldWindow.m_window;
      m_renderer = oldWindow.m_renderer;
    }
   
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
  };

  class FontManager;

  static const WindowParams defaultScreen = {"CapEngine",1280,800,32,false, false,false};


  class VideoManager {
  public:
    VideoManager();    
    VideoManager(Logger* loggerIn);
    void initSystem(WindowParams windowParams);
    Uint32 createNewWindow(WindowParams windowParams);
    void closeWindow(Uint32 windowID);
    void shutdown();
    void clearScreen();
    void drawScreen();
    void getWindowResolution(int* width, int* height) const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    void setBackgroundColour(Colour colour);
    // Surfaces
    Surface* loadSurface(std::string fileName) const;
    Surface* createSurface(int width, int height);
    void closeSurface(Surface* surface) const;
    void blitSurface(Surface* sourceSurface, int srcX, int srcY,  int sourceWidth, int sourceHeight, Surface* destSurface, int x, int y);
    real getSurfaceWidth(const Surface* surface) const;
    real getSurfaceHeight(const Surface* surface) const;
    // Textures
    Texture* loadImage(std::string fileName) const;
    void closeTexture(Texture* texture) const;
    void drawTexture(int x, int y, Texture* texture, Rect* srcRect=nullptr) const;
    void drawTexture(Texture* texture, Rect* srcRect, Rect* dstRect) const;
    real getTextureWidth(Texture* texture) const;
    real getTextureHeight(Texture* texture) const;
    void getTextureDims(Texture* texture, int* x, int* y) const;
    Texture* createTexture(int width, int height);
    Texture* createTextureFromSurface(Surface* surface, bool freeSurface = false); 

    //opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath="", Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

    // input
    void loadControllerMapFromFile(std::string filePath);
    //Drawing
    void drawRect(Rect rect, Colour fillColour);

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

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    std::map<Uint32, Window> m_windows;
    
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
