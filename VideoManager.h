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
    Texture* loadImage(std::string fileName) const;
    void closeTexture(Texture* texture) const;
    void drawTexture(int x, int y, Texture* texture, Rect* srcRect=nullptr) const;
    void drawTexture(Texture* texture, Rect* srcRect, Rect* dstRect) const;
    void shutdown();
    void drawScreen();
    void getWindowResolution(int* width, int* height) const;
    real getTextureWidth(Texture* texture) const;
    real getTextureHeight(Texture* texture) const;
    Texture* createTexture(int width, int height);
    void blitTextures(Texture* sourceTexture, int srcX, int srcY,  int sourceWidth, int sourceHeight, Texture* destTexture, int x, int y);
    //opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath="", Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

    bool initialized;

  protected:
    void setColorKey(Surface* surface) const;

  private:
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
