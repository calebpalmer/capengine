#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

//@brief - header file for VideoManager class
//@author - Caleb Palmer

#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "captypes.h"
#include "CapEngineException.h"
#include "Time.h"
#include "fontmanager.h"
#include "logger.h"

namespace CapEngine {

  struct Screen_t{
    int width;
    int height;
    int pDepth;
    bool fullScreen;
    bool opengl;
  };

  class FontManager;

  const Screen_t defaultScreen = {1280,800,32,false};


  class VideoManager {
  public:
    VideoManager();    
    VideoManager(Logger* loggerIn);
    void initSystem(Screen_t screenConfig);
    void shutdown();
    void clearScreen();
    void drawScreen();
    void getWindowResolution(int* width, int* height) const;
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
    Texture* createTexture(int width, int height);
    Texture* createTextureFromSurface(Surface* surface); // TODO

    //opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath="", Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

    // input
    void loadControllerMapFromFile(std::string filePath);

    bool initialized;

  protected:
    void setColorKey(Surface* surface) const;
    void loadControllerMaps();

  private:
    // private for singleton
    VideoManager(const VideoManager& videoManager);
    VideoManager& operator=(const VideoManager& videoManager);


    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    Logger* logger;
    static bool instantiated; //singleton
    Screen_t currentScreenConfig;
    void (*reshapeFunc)(int, int); //for opengl resize functions
    CapEngine::Time lastRenderTime;
    float fps;
    std::unique_ptr<CapEngine::FontManager> up_fontManager;

    bool showFPS;
    std::string ttfFontPath;
    Uint8 fpsColourR;
    Uint8 fpsColourG;
    Uint8 fpsColourB;
  };

}

#endif
