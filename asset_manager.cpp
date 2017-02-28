#include "asset_manager.h"

#include <vector>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <cassert>

#include "CapEngineException.h"
#include "xml_parser.h"

using namespace std;
using namespace CapEngine;

namespace {

  std::map<string, Frame> parseFrames(XmlParser& parser, XmlNode parentNode){
    std::map<string, Frame>  frameMap;
    vector<XmlNode> childNodes = parser.getNodeChildren(parentNode);
    
    for(auto && node : childNodes){
      if(parser.nodeNameCompare(node, "row")){
	// read the attributes of the frame
	string frameName = parser.getAttribute(node ,"frameName");
	string rowNumStr = parser.getAttribute(node, "rowNum");
	string frameWidthStr = parser.getAttribute(node, "frameWidth");
	string frameHeightStr = parser.getAttribute(node,"frameHeight");
	string numFramesStr = parser.getAttribute(node,  "numFrames");
	string animationTime = parser.getAttribute(node, "animationTime");

	int horizontalPadding = 0;
	int verticalPadding = 0;
	try{
	  horizontalPadding = std::stoi(parser.getAttribute(node, "horizontalPadding"));
	  verticalPadding = std::stoi(parser.getAttribute(node, "verticalPadding"));
	}
	catch(...){
	  // ignore any errors because these are optional
	}

	// make sure there is a frameName
	if(frameName == ""){
	  throw CapEngineException("Frame name missing while reading Asset file");
	}

	Frame frame = {frameName, std::stoi(rowNumStr), std::stoi(frameWidthStr), std::stoi(frameHeightStr),
		       std::stoi(numFramesStr), std::stod(animationTime), horizontalPadding, verticalPadding};

	// does frame under this name aready exist?
	if(frameMap.find(frameName) != frameMap.end()){
	  ostringstream msg;
	  msg << "Frame name \"" << frameName << "\" already exists";
	  throw CapEngineException(msg.str());
	}
	frameMap[frameName] = frame;
      }
    }

    return frameMap;
  }
  
} // end anonymous namespace

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
  mImageMap[id] = image;
}

void AssetManager::parseAssetFile(XmlParser& parser){
  // get Images nodes at /assets/images/image
 vector<XmlNode> images = parser.getNodes("/assets/textures/texture");
  auto imageIter = images.begin();
  for( ; imageIter != images.end(); imageIter++){
    string id = parser.getAttribute(*imageIter, "id");
    string path = parser.getStringValue(*imageIter);
    string frameWidth = parser.getAttribute(*imageIter, "frameWidth");
    string frameHeight = parser.getAttribute(*imageIter, "frameHeight");
    string hasFrames = parser.getAttribute(*imageIter, "hasFrames");
    
    //convert id to int
    istringstream idStream(id);
    int tId;
    idStream >> tId;

    Image image;
    image.path = path;
    image.texture = nullptr;
    if(hasFrames == "y"){
      image.frames = parseFrames(parser, *imageIter);
    }

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

SoftwareImage AssetManager::getSoftwareImage(int id){
  auto iter = mImageMap.find(id);
  if(iter == mImageMap.end()){
    ostringstream message;
    message << "Image with id " << id << " does not exist";
    throw CapEngineException(message.str());
  }

  Surface* surface = mVideoManager.loadSurface(iter->second.path);
  SoftwareImage softwareImage;
  softwareImage.path = iter->second.path;
  softwareImage.surface = surface;

  return softwareImage;
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

Frame AssetManager::getFrame(int assetID, std::string frameName){
  Image* pImage = this->getImage(assetID);
  assert(pImage != nullptr);

  auto frameIter = pImage->frames.find(frameName);
  if(frameIter == pImage->frames.end()){
    std::ostringstream msg;
    msg << "Frame " << frameName << " does not exist";
    throw CapEngineException(msg.str());
  }

  return frameIter->second;
}

Frame AssetManager::getFrame(int assetID, int rowNum){
  Image* pImage = this->getImage(assetID);
  assert(pImage != nullptr);

  Frame frame;
  bool found = false;
  for(auto && i : pImage->frames){
    if(i.second.rowNum == rowNum){
      frame = i.second;
      found = true;
    }
  }

  if(!found){
    std::ostringstream msg;
    msg << "Frame " << rowNum << " does not exist";
    throw CapEngineException(msg.str());
  }

  return frame;
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

void AssetManager::draw(Uint32 windowID, int id, Rectangle _srcRect, Rectangle _destRect){
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
  
  mVideoManager.drawTexture(windowID, image->texture, &srcRect, &destRect);
}

void AssetManager::draw(Uint32 windowID, int id, Vector position){
  Image* image = this->getImage(id);

  Rect destRect;
  destRect.x = position.x;
  destRect.y = position.y;
  destRect.w = mVideoManager.getTextureWidth(image->texture);
  destRect.h = mVideoManager.getTextureHeight(image->texture);

  mVideoManager.drawTexture(windowID, image->texture, nullptr, &destRect);
  
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle destRect){
  Image* image = this->getImage(id);
  Rect rect = {destRect.x, destRect.y, destRect.width, destRect.height};
  mVideoManager.drawTexture(windowID, image->texture, nullptr, &rect);
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle _destRect, int row, int frameNum){
  Image* image = this->getImage(id);
  Frame frame = this->getFrame(id, row);
  // need to add some checkingto make sure row and frames exists
  
  Rect srcRect;
  srcRect.x = frameNum * frame.frameWidth;
  srcRect.y = row * frame.frameHeight;
  srcRect.w = frame.frameWidth;
  srcRect.h = frame.frameHeight;

  Rect destRect;
  destRect.x = _destRect.x;
  destRect.y = _destRect.y;
  destRect.w = _destRect.width;
  destRect.h = _destRect.height;

  mVideoManager.drawTexture(windowID, image->texture, &srcRect, &destRect);
  
}

long AssetManager::playSound(int id, bool repeat){
    // TODO implement repeat functionality
    Sound* sound = getSound(id); 
    unique_ptr<PCM> upTempPCM(new PCM(*(sound->pcm)));
    long soundID = mSoundPlayer.addSound(upTempPCM.release(), repeat);
    return soundID;
}

void AssetManager::stopSound(int id){
  mSoundPlayer.deleteSound(id);
}

