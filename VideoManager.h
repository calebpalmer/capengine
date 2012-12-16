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

namespace CapEngine {

  struct Screen_t{
    int width;
    int height;
    int pDepth;
  };

  const Screen_t defaultScreen = {1280,800,32};


  class VideoManager {
    SDL_Surface *mainSurface;
      
  public:
    //default constructor for the VideoManager class
    VideoManager();

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
    
  };

}

#endif
