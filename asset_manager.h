#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <map>

#include "captypes.h"
#include "pcm.h"
#include "VideoManager.h"
#include "soundplayer.h"
#include "xml_parser.h"

namespace CapEngine{

  struct Texture {
    std::string path;
    Surface* surface;
    int frameWidth;
    int frameHeight;
  };

  struct Sound {
    std::string path;
    PCM* pcm;
  };
  
  class AssetManager{
    
  public:
    AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, std::string assetFile);
    ~AssetManager();
    void draw(int id, Rect srcRect, Rect dstRect);
    void draw(int id, Rect dstRect, int row, int frame);
    void loadTexture(int id, std::string path, int frameWidth=0, int frameHeight=0);
    Texture* getTexture(int id); 
    void playSound(int id, bool repeat);
    void stopSound(int id);
    void loadSound(int id, std::string path);
    Sound* getSound(int id);

  private: 
    std::map<int, Texture> mTextureMap;
    std::map<int, Sound> mSoundMap;
    VideoManager& mVideoManager;
    SoundPlayer& mSoundPlayer;
    std::string mAssetFile;

  private: //functions
    void parseAssetFile(XmlParser& parser);

  };

}
#endif // ASSET_MANAGER_H
