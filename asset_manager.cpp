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
  // free textures in image map
  auto tIter = mImageMap.begin();
  while(tIter != mImageMap.end()){
    if(tIter->second.texture != nullptr){
      mVideoManager.closeTexture(tIter->second.texture);
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

void AssetManager::loadImage(int id, string path, int frameWidth, int frameHeight){
  Texture* tempTexture = mVideoManager.loadImage(path);
  if(tempTexture == nullptr){
    throw CapEngineException("Unable to load image at " + path);
  }

  if(mImageMap.find(id) != mImageMap.end()){
    ostringstream errorStream;
    errorStream << "Image under id " << id << " already exists.";
    throw CapEngineException(errorStream.str());
  }
  
  Image image;
  image.path = path;
  image.texture = tempTexture;
  image.frameWidth = frameWidth;
  image.frameHeight = frameHeight;
  mImageMap[id] = image;
}

void AssetManager::parseAssetFile(XmlParser& parser){
  // get Images nodes at /assets/images/image
 vector<XmlNode> images = parser.getNodes("/assets/images/image");
  auto imageIter = images.begin();
  for( ; imageIter != images.end(); imageIter++){
    string id = parser.getAttribute(*imageIter, "id");
    string path = parser.getStringValue(*imageIter);
    string frameWidth = parser.getAttribute(*imageIter, "frameWidth");
    string frameHeight = parser.getAttribute(*imageIter, "frameHeight");
    
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

    Image image;
    image.path = path;
    image.texture = nullptr;
    image.frameWidth = fWidth;
    image.frameHeight = fHeight;

    mImageMap[tId] = image;
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

Image* AssetManager::getImage(int id){
  // throw error if image has not been loaded
  auto iter = mImageMap.find(id);
  if(iter == mImageMap.end()){
    ostringstream message;
    message << "Image with id " << id << " does not exist";
    throw CapEngineException(message.str());
  }

  if(iter->second.texture == nullptr){
    iter->second.texture = mVideoManager.loadImage(iter->second.path);
    if(iter->second.texture == nullptr){
      throw CapEngineException("Unable to load image at " + iter->second.path);
    }
  }
  return &(iter->second);
}

int AssetManager::getImageWidth(int id){
  Image* image = this->getImage(id);

  int width;
  width = mVideoManager.getTextureWidth(image->texture);
  return width;
}

int AssetManager::getImageHeight(int id){
  Image* image = this->getImage(id);

  int height;
  height = mVideoManager.getTextureHeight(image->texture);
  return height;
}

Sound* AssetManager::getSound(int id){
  // throw error if image has not been loaded
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
      throw CapEngineException("Unable to load image at " + iter->second.path);
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

void AssetManager::draw(int id, Rectangle _srcRect, Rectangle _destRect){
  Image* image = this->getImage(id);

  Rect srcRect;
  srcRect.x = _srcRect.x;
  srcRect.y = _srcRect.y;
  srcRect.w = _srcRect.width;
  srcRect.h = _srcRect.height;

  Rect destRect;
  destRect.x = _destRect.x;
  destRect.y = _destRect.y;
  destRect.w = _destRect.width;
  destRect.h = _destRect.height;
  
  mVideoManager.drawTexture(image->texture, &srcRect, &destRect);
}

void AssetManager::draw(int id, Vector position){
  Image* image = this->getImage(id);

  Rect destRect;
  destRect.x = position.x;
  destRect.y = position.y;

  mVideoManager.drawTexture(image->texture, nullptr, &destRect);
}

void AssetManager::draw(int id, Rectangle _destRect, int row, int frame){
  Image* image = this->getImage(id);
  real width, height;
  width = mVideoManager.getTextureWidth(image->texture);
  height = mVideoManager.getTextureHeight(image->texture);

  if(image->frameWidth == 0 || image->frameHeight == 0){
    throw CapEngineException("Image does not contain frames");
  }

  if(frame * image->frameWidth > width - image->frameWidth){
    throw CapEngineException("Frame out of bounds of image");
  }

  if(row * image->frameHeight > height - image->frameHeight){
    throw CapEngineException("Row out of bounds of image");
  }

  Rect srcRect;
  srcRect.x = frame * image->frameWidth;
  srcRect.y = row * image->frameHeight;
  srcRect.w = image->frameWidth;
  srcRect.h = image->frameHeight;

  Rect destRect;
  destRect.x = _destRect.x;
  destRect.y = _destRect.y;
  destRect.w = _destRect.width;
  destRect.h = _destRect.height;

  mVideoManager.drawTexture(image->texture, &srcRect, &destRect);
  
}

void AssetManager::playSound(int id, bool repeat){
    // TODO implement repeat functionality
    Sound* sound = getSound(id); 
    unique_ptr<PCM> upTempPCM(new PCM(*(sound->pcm)));
    mSoundPlayer.addSound(upTempPCM.release(), repeat);
}
