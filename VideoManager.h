#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

//@brief - header file for VideoManager class
//@author - Caleb Palmer

#include <string>
#include <memory>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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
    bool opengl;
  };

  class FontManager;

  const Screen_t defaultScreen = {1280,800,32,false};


  class VideoManager {
    // private for singleton
    VideoManager(const VideoManager& videoManager);
    VideoManager& operator=(const VideoManager& videoManager);

  public:
    VideoManager();    
    VideoManager(Logger* loggerIn);
    void initSystem(Screen_t screenConfig);
    Surface* loadImage(std::string fileName) const;
    void setColorKey(Surface* surface) const;
    void closeSurface(Surface* surface) const;
    void drawSurface(int x, int y, Surface* source, Rect* rect=nullptr) const;
    void shutdown();
    void drawScreen();
    void getWindowResolution(int* width, int* height) const;
    real getSurfaceWidth(const Surface* surface) const;
    real getSurfaceHeight(const Surface* surface) const;
    Surface* createSurface(int width, int height);
    void blitSurface(Surface& sourceSurface, int srcX, int srcY,  int sourceWidth, int sourceHeight, Surface& destSurface, int x, int y);
    //opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath="");

  public:
    bool initialized;
    
  private:
    SDL_Surface *mainSurface;
    Logger* logger;
    static bool instantiated; //singleton
    Screen_t currentScreenConfig;
    void (*reshapeFunc)(int, int); //for opengl resize functions
    CapEngine::Time lastRenderTime;
    float fps;
    std::unique_ptr<CapEngine::FontManager> up_fontManager;
    bool showFPS;
    std::string ttfFontPath;
  };

}

#endif
