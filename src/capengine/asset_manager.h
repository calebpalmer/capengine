#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>

#include "CapEngineException.h"
#include "VideoManager.h"
#include "captypes.h"
#include "collision.h"  // Rectangle definition
#include "pcm.h"
#include "soundplayer.h"
#include "vector.h"
#include "xml_parser.h"

namespace CapEngine {

struct Frame {
    std::string frameName;
    int rowNum;
    int frameWidth;
    int frameHeight;
    int numFrames;
    double animationTime;
    int horizontalPadding;
    int verticalPadding;
};

struct Image {
    std::string path;
    Texture* texture;
    std::map<std::string, Frame> frames;
};

struct AnimatedImage {
    std::string path;
    std::shared_ptr<Texture> texture;
    int numFrames;
    int animationTimeMs;
};

struct SoftwareImage {
    std::string path;
    Surface* surface;
};

struct Sound {
    std::string path;
    PCM* pcm;
};

struct AssetDoesNotExistError : public CapEngineException {
    AssetDoesNotExistError(std::string_view in_type, int in_assetId)
        : CapEngineException([&]() {
              std::stringstream stream;
              stream << "The " << in_type << " with id " << in_assetId
                     << " does not exist.";
              return stream.str();
          }())
    {
    }
};

class AssetManager {
   public:
    AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer,
                 std::optional<std::string> assetFile, std::optional<std::filesystem::path> in_basePath = std::nullopt);
    ~AssetManager();
    void draw(Uint32 windowID, int id, Rectangle srcRect, Rectangle dstRect,
              std::optional<double> rotationDegrees = std::nullopt);
    void draw(Uint32 windowID, int id, Rectangle dstRect, int row, int frame);
    void draw(Uint32 windowID, int id, Vector position);
    void draw(Uint32 windowID, int id, Rectangle dstRect);
    void loadImage(int id, std::string path, int frameWidth = 0,
                   int frameHeight = 0);
    Image* getImage(int id);
    std::optional<AnimatedImage> getAnimatedImage(int in_id);
    SoftwareImage getSoftwareImage(int id);
    [[nodiscard]] bool imageExists(int id) const;
    int getImageWidth(int id);
    int getImageHeight(int id);
    Frame getFrame(int assetID, std::string frameName);
    Frame getFrame(int assetID, int rowNum);

    int64_t playSound(int id, bool repeat = false);
    void stopSound(int id);
    void loadSound(int id, std::string path);
    Sound* getSound(int id);
    [[nodiscard]] bool soundExists(int id) const;

    [[nodiscard]] std::optional<std::filesystem::path> getBasePath() const;

   private:
    std::map<int, Image> m_imageMap;
    std::map<int, AnimatedImage> m_animationMap;
    std::map<int, Sound> m_soundMap;
    VideoManager& m_videoManager;
    SoundPlayer& m_soundPlayer;
    std::optional<std::string> m_assetFile;
    std::optional<std::filesystem::path> m_basePath;

   private:  // functions
    void parseAssetFile(XmlParser& parser);
};

}  // namespace CapEngine
#endif  // ASSET_MANAGER_H
