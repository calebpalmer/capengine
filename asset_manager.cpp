#include "asset_manager.h"

#include <sstream>

#include "CapEngineException.h"

using namespace std;

AssetManager::AssetManage(VideoManager& videoManager, SoundPlayer& soundPlayer, string assetFile)
  : mAssetFile(assetFile), mVideoManager(videoManager), mSoundPlayer(soundPlayer)
{
  // read asset file and load images/sounds
}

AssetManager::~AssetManager(){
  // free surfaces in texture map
  // free sounds in sound map
}

void AssetManager::loadTexture(int id, string path){
  Surface* tempSurface = mVideoManager.loadImage(path);
  if(tempSurface == null_ptr){
    throw CapEngineException("Unable to load texture at " + path);
  }

  if(mTextureMap.find(id) != mTextureMap.end()){
    ostringstream errorStream;
    errorStream << "Texture under id " << id << " already exists.";
    throw CapEngineException(errorStream.str());
  }

  mTextureMap[id] = tempSurface;
}
