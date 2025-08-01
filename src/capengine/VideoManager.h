#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

//@brief - header file for VideoManager class
//@author - Caleb Palmer

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_video.h>

#include <memory>
#include <optional>
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
	bool bordered = false;
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

class VideoManager final {
   public:
    VideoManager();
    explicit VideoManager(Logger* loggerIn);
    ~VideoManager() = default;

    Uint32 initSystem(WindowParams windowParams, bool noWindow = false);
    std::vector<Uint32> getWindows() const;
    void shutdown();
    void clearScreen(Uint32 windowID);
    void drawScreen(Uint32 windowID);
    void clearAll();
    void drawAll();

    // Windows
    Uint32 createNewWindow(WindowParams windowParams);
    void closeWindow(Uint32 windowID);
    Window getWindow(Uint32 windowID);
    Uint32 getWindowId(const std::string& windowName) const;
    bool isValidWindowId(Uint32 windowId) const;
    std::pair<int, int> getWindowResolution(Uint32 windowID);
    std::pair<int, int> getWindowLogicalResolution(uint32_t in_windowID);
    void setWindowLogicalResolution(uint32_t in_windowID, int in_width, int in_height);
    int getWindowWidth(Uint32 windowID);
    int getWindowHeight(Uint32 windowID);
    void setWindowPosition(Uint32 windowId, int x, int y);
    void setWindowSize(Uint32 windowId, int width, int height);
    void maximizeWindow(Uint32 windowId);
    void setWindowFullScreen(Uint32 windowId, bool fullScreen);

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
    SurfacePtr createSurfaceFromTexture(Texture* in_texture);
    static void replaceColour(Surface* in_surface, Colour in_oldColour, Colour in_newColour);
    void saveSurface(Surface* surface, const std::string& filePath);
    void closeSurface(Surface* surface) const;
    void blitSurface(Surface* sourceSurface, int srcX, int srcY, int sourceWidth, int sourceHeight,
                     Surface* destSurface, int x, int y);
    double getSurfaceWidth(const Surface* surface) const;
    double getSurfaceHeight(const Surface* surface) const;
    // Textures
    Texture* loadImage(std::string fileName) const;
    TexturePtr loadImagePtr(std::string const& in_filePath) const;
    std::shared_ptr<Texture> loadSharedImage(std::string const& in_filePath) const;
    void closeTexture(Texture* texture) const;
    void drawTexture(Uint32 windowID, Rect dstRect, Texture* texture, Rect* srcRect = nullptr,
                     bool applyTransform = true);
    void drawTexture(Uint32 windowID, Texture* texture, Rect* srcRect, Rect* dstRect,
                     std::optional<double> rotationDegrees = std::nullopt, SDL_RendererFlip flip = SDL_FLIP_NONE,
                     bool applyTransform = true);
    void drawTexture(Texture* in_dstTexture, Texture* in_srcTexture, Rect& in_dstRect, Rect& in_srcRect);
    double getTextureWidth(Texture* texture) const;
    double getTextureHeight(Texture* texture) const;
    void getTextureDims(Texture* texture, int* x, int* y) const;
    std::pair<int, int> getTextureDims(Texture* texture) const;
    Texture* createTexture(int width, int height, Colour fillColour = Colour{0, 0, 0, 0});
    TexturePtr createTexturePtr(int width, int height, Colour fillColour = Colour{0, 0, 0, 0});
    Texture* createTextureFromSurface(Surface* surface, bool freeSurface = false);
    Texture* createTextureFromSurface(Uint32 windowID, Surface* surface, bool freeSurface = false);
    TexturePtr createTextureFromSurfacePtr(Surface* surface, bool freeSurface = false);
    TexturePtr createTextureFromSurfacePtr(Uint32 windowId, Surface* surface, bool freeSurface = false);
    TexturePtr copyTexture(Texture* sourceTexture);
    void saveTexture(Texture* texture, const std::string& filePath);
    void setClipRect(Uint32 windowId, SDL_Rect const* clipRect);

    // opengl support
    void setReshapeFunc(void (*func)(int x, int y));
    void callReshapeFunc(int w, int h);
    void displayFPS(bool on, const std::string& ttfFontPath = "", Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);

    // input
    void loadControllerMapFromFile(std::string filePath);
    // Drawing
    void drawLine(Uint32 windowID, int x1, int y1, int x2, int y2, const Colour& strokeColour);
    void drawFillRect(Uint32 windowID, Rect rect, Colour fillColour);
    void drawFillRect(Texture* in_texture, Rect rect, Colour fillColour);
    void drawRect(Uint32 windowID, Rect rect, Colour fillColour);

    // utility
    int toScreenCoord(const Surface* surface, int y) const;
    int fromScreenCoord(const Surface* surface, int y) const;

    SDL_Renderer* getRenderer();

    bool initialized = false;
    const std::string mainWindowName = "main";

    static const int kInvalidWindowId = 0;  //<! The of an invalid window

   protected:
    void setColorKey(Surface* surface) const;
    void loadControllerMaps();

   private:
    VideoManager(const VideoManager& videoManager);
    VideoManager& operator=(const VideoManager& videoManager);

    WindowPtr createWindow(WindowParams windowParams);
    RendererPtr createRenderer(SDL_Window* window, WindowParams windowParams);

    WindowPtr m_window;
    RendererPtr m_renderer;
    std::map<Uint32, Window> m_windows;
    std::map<std::string, Uint32> m_windowNamesToIds;

    Matrix m_transformationMatrix;

    std::unique_ptr<Logger> logger;
    static bool instantiated;  // singleton
    WindowParams currentWindowParams;
    void (*reshapeFunc)(int, int);  // for opengl resize functions
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

}  // namespace CapEngine

#endif
