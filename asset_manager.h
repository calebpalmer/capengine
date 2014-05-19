#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <map>

#include "captypes.h"
#include "pcm.h"
#include "VideoManager.h"
#include "soundplayer.h"

namespace CapEngine{
  
  class AssetManager{
    
  public:
    AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, std::string assetFile);
    ~AssetManager();
    void draw(int id, Rect srcRect, Rect dstRect);
    void draw(int id, Rect srcRect, Rect dstRect, int row, int frame);
    void loadTexture(int id, std::string path);
    void playSound(int id, bool repeat);
    void stopSound(int id);
    void loadSound(int id, std::string path);

  private:
    std::map<int, Surface*> mTextureMap;
    std::map<int, PCM*> mSoundMap;
    VideoManager& mVideoManager;
    SoundPlayer& mSoundPlayer;
    std::string mAssetFile;

  };

}
#endif // ASSET_MANAGER_H
