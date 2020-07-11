#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "CapEngineException.h"
#include "VideoManager.h"
#include "captypes.h"
#include "collision.h" // Rectangle definition
#include "pcm.h"
#include "soundplayer.h"
#include "vector.h"
#include "xml_parser.h"

#include <filesystem>
#include <map>
#include <string>

namespace CapEngine
{

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
  Texture *texture;
  std::map<std::string, Frame> frames;
};

struct SoftwareImage {
  std::string path;
  Surface *surface;
};

struct Sound {
  std::string path;
  PCM *pcm;
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

class AssetManager
{

public:
  AssetManager(VideoManager &videoManager, SoundPlayer &soundPlayer,
               const std::string &assetFile);
  ~AssetManager();
  void draw(Uint32 windowID, int id, Rectangle srcRect, Rectangle dstRect);
  void draw(Uint32 windowID, int id, Rectangle dstRect, int row, int frame);
  void draw(Uint32 windowID, int id, Vector position);
  void draw(Uint32 windowID, int id, Rectangle dstRect);
  void loadImage(int id, std::string path, int frameWidth = 0,
                 int frameHeight = 0);
  Image *getImage(int id);
  SoftwareImage getSoftwareImage(int id);
  bool imageExists(int id) const;
  int getImageWidth(int id);
  int getImageHeight(int id);
  Frame getFrame(int assetID, std::string frameName);
  Frame getFrame(int assetID, int rowNum);

  long playSound(int id, bool repeat = false);
  void stopSound(int id);
  void loadSound(int id, std::string path);
  Sound *getSound(int id);
  bool soundExists(int id) const;

private:
  std::map<int, Image> mImageMap;
  std::map<int, Sound> mSoundMap;
  VideoManager &mVideoManager;
  SoundPlayer &mSoundPlayer;
  std::string mAssetFile;

private: // functions
  void parseAssetFile(XmlParser &parser);
};

} // namespace CapEngine
#endif // ASSET_MANAGER_H
