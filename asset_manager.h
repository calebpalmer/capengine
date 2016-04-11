#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <map>

#include "captypes.h"
#include "pcm.h"
#include "VideoManager.h"
#include "soundplayer.h"
#include "xml_parser.h"
#include "collision.h" // Rectangle definition
#include "vector.h" 

namespace CapEngine{

  struct Image {
    std::string path;
    Texture* texture;
    int frameWidth;
    int frameHeight;
  };

  struct SoftwareImage{
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
    void draw(int id, Rectangle srcRect, Rectangle dstRect);
    void draw(int id, Rectangle dstRect, int row, int frame);
    void draw(int id, Vector position);
    void loadImage(int id, std::string path, int frameWidth=0, int frameHeight=0);
    Image* getImage(int id);
    SoftwareImage getSoftwareImage(int id);
    int getImageWidth(int id);
    int getImageHeight(int id);
    long playSound(int id, bool repeat=false);
    void stopSound(int id);
    void loadSound(int id, std::string path);
    Sound* getSound(int id);

  private: 
    std::map<int, Image> mImageMap;
    std::map<int, Sound> mSoundMap;
    VideoManager& mVideoManager;
    SoundPlayer& mSoundPlayer;
    std::string mAssetFile;

  private: //functions
    void parseAssetFile(XmlParser& parser);

  };

}
#endif // ASSET_MANAGER_H
