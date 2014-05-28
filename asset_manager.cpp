#include "asset_manager.h"

#include <vector>
#include <sstream>
#include <memory>
#include <stdexcept>

#include "CapEngineException.h"
#include "xml_parser.h"


using namespace std;
using namespace CapEngine;

AssetManager::AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, string assetFile)
  : mVideoManager(videoManager), mSoundPlayer(soundPlayer), mAssetFile(assetFile)
{
XmlParser parser(assetFile);
 parseAssetFile(parser);
}
  
AssetManager::~AssetManager(){
  // free surfaces in texture map
  auto tIter = mTextureMap.begin();
  while(tIter != mTextureMap.end()){
    if(tIter->second.surface != nullptr){
      mVideoManager.closeSurface(tIter->second.surface);
    }
    tIter++;
  }
  // free sounds in sound map
  auto sIter = mSoundMap.begin();
  while(sIter != mSoundMap.end()){
    if(sIter->second.pcm != nullptr){
      delete sIter->second.pcm;
    }
    sIter++;
  }
}

void AssetManager::loadTexture(int id, string path, int frameWidth, int frameHeight){
  Surface* tempSurface = mVideoManager.loadImage(path);
  if(tempSurface == nullptr){
    throw CapEngineException("Unable to load texture at " + path);
  }

  if(mTextureMap.find(id) != mTextureMap.end()){
    ostringstream errorStream;
    errorStream << "Texture under id " << id << " already exists.";
    throw CapEngineException(errorStream.str());
  }
  
  Texture texture;
  texture.path = path;
  texture.surface = tempSurface;
  texture.frameWidth = frameWidth;
  texture.frameHeight = frameHeight;
  mTextureMap[id] = texture;
}

void AssetManager::parseAssetFile(XmlParser& parser){
  // get Textures nodes at /assets/textures/texture
  vector<XmlNode> textures = parser.getNodes("/assets/textures/texture");
  auto textureIter = textures.begin();
  for( ; textureIter != textures.end(); textureIter++){
    string id = parser.getAttribute(*textureIter, "id");
    string path = parser.getStringValue(*textureIter);
    string frameWidth = parser.getAttribute(*textureIter, "frameWidth");
    string frameHeight = parser.getAttribute(*textureIter, "frameHeight");
    
    //convert id to int
    istringstream idStream(id);
    int tId;
    idStream >> tId;

    // stoi introduced in c++11
    int fWidth, fHeight = 0;
    try{
      fWidth = stoi( frameWidth );
    }
    catch(const invalid_argument& e){
      fWidth = 0;
    }

    try{
      fHeight = stoi( frameHeight );
    }
    catch(const invalid_argument& e){
      fWidth = 0;
    }

    Texture texture;
    texture.path = path;
    texture.surface = nullptr;
    texture.frameWidth = fWidth;
    texture.frameHeight = fHeight;

    mTextureMap[tId] = texture;
  }

  // get sounds at /assets/sounds
  vector<XmlNode> sounds = parser.getNodes("/assets/sounds/sound");
  auto soundIter = sounds.begin();
  for ( ; soundIter != sounds.end(); soundIter++){
    string id = parser.getAttribute(*soundIter, "id");
    string path = parser.getStringValue(*soundIter);
    
    //convert id to int
    istringstream idStream(id);
    int sId;
    idStream >> sId;
    
    Sound  sound;
    sound.path = path;
    sound.pcm = nullptr;

    mSoundMap[sId] = sound;
  }
  
}

Texture* AssetManager::getTexture(int id){
  // throw error if texture has not been loaded
  auto iter = mTextureMap.find(id);
  if(iter == mTextureMap.end()){
    ostringstream message;
    message << "Texture with id " << id << " does not exist";
    throw CapEngineException(message.str());
  }

  if(iter->second.surface == nullptr){
    iter->second.surface = mVideoManager.loadImage(iter->second.path);
    if(iter->second.surface == nullptr){
      throw CapEngineException("Unable to load texture at " + iter->second.path);
    }
  }
  return &(iter->second);

}

Sound* AssetManager::getSound(int id){
  // throw error if texture has not been loaded
  auto iter = mSoundMap.find(id);
  if(iter == mSoundMap.end()){
    ostringstream message;
    message << "Sound with id " << id << " does not exist";
    throw CapEngineException(message.str());
  }

  if(iter->second.pcm == nullptr){
    unique_ptr<PCM> upPcm(new PCM(iter->second.path));
    iter->second.pcm = upPcm.release();
    if(iter->second.pcm == nullptr){
      throw CapEngineException("Unable to load texture at " + iter->second.path);
    }
  }
  return &(iter->second);
}

void AssetManager::loadSound(int id, string path){
  unique_ptr<PCM> upTempPCM(new PCM(path));  // throws exception if failure

  if(mSoundMap.find(id) != mSoundMap.end()){
    ostringstream errorStream;
    errorStream << "Sound under id " << id << " already exists.";
    throw CapEngineException(errorStream.str());
  }
  
  Sound sound;
  sound.path = path;
  sound.pcm = upTempPCM.release();
  mSoundMap[id] = sound;

}
