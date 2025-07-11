// TODO This whole darn file needs refactoring
#include "asset_manager.h"

#include <cassert>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "CapEngineException.h"
#include "locator.h"
#include "xml_parser.h"

using namespace std;

namespace CapEngine {

namespace {

std::map<string, Frame> parseFrames(XmlParser& parser, XmlNode parentNode)
{
    std::map<string, Frame> frameMap;
    vector<XmlNode> childNodes = parser.getNodeChildren(parentNode);

    for (auto&& node : childNodes) {
        if (parser.nodeNameCompare(node, "row")) {
            // read the attributes of the frame
            string frameName = parser.getAttribute(node, "frameName");
            string rowNumStr = parser.getAttribute(node, "rowNum");
            string frameWidthStr = parser.getAttribute(node, "frameWidth");
            string frameHeightStr = parser.getAttribute(node, "frameHeight");
            string numFramesStr = parser.getAttribute(node, "numFrames");
            string animationTime = parser.getAttribute(node, "animationTime");

            int horizontalPadding = 0;
            int verticalPadding = 0;
            try {
                horizontalPadding = std::stoi(parser.getAttribute(node, "horizontalPadding"));
                verticalPadding = std::stoi(parser.getAttribute(node, "verticalPadding"));
            }
            catch (...) {
                // ignore any errors because these are optional
            }

            // make sure there is a frameName
            if (frameName == "") {
                throw CapEngineException("Frame name missing while reading Asset file");
            }

            Frame frame = {frameName,
                           std::stoi(rowNumStr),
                           std::stoi(frameWidthStr),
                           std::stoi(frameHeightStr),
                           std::stoi(numFramesStr),
                           std::stod(animationTime),
                           horizontalPadding,
                           verticalPadding};

            // does frame under this name aready exist?
            if (frameMap.find(frameName) != frameMap.end()) {
                ostringstream msg;
                msg << "Frame name \"" << frameName << "\" already exists";
                throw CapEngineException(msg.str());
            }
            frameMap[frameName] = frame;
        }
    }

    return frameMap;
}

}  // end anonymous namespace

AssetManager::AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, std::optional<string> assetFile,
                           std::optional<std::filesystem::path> basePath)
    : m_videoManager(videoManager), m_soundPlayer(soundPlayer), m_assetFile(assetFile), m_basePath(basePath)
{
    if (m_assetFile.has_value()) {
        m_assetFile = std::filesystem::absolute(*assetFile).string();
        XmlParser parser(*assetFile);
        parseAssetFile(parser);

        // if base path is not provided, set it from the asset file
        if (!m_basePath.has_value()) {
            m_basePath = std::filesystem::path{*m_assetFile}.parent_path();
        }
    }
}

AssetManager::~AssetManager()
{
    // free textures in image map
    auto tIter = m_imageMap.begin();
    while (tIter != m_imageMap.end()) {
        if (tIter->second.texture != nullptr) {
            m_videoManager.closeTexture(tIter->second.texture);
        }
        tIter++;
    }
    // free sounds in sound map
    auto sIter = m_soundMap.begin();
    while (sIter != m_soundMap.end()) {
        if (sIter->second.pcm != nullptr) {
            delete sIter->second.pcm;
        }
        sIter++;
    }
}

void AssetManager::loadImage(int id, string path, int frameWidth, int frameHeight)
{
    Texture* tempTexture = m_videoManager.loadImage(path);
    if (tempTexture == nullptr) {
        throw CapEngineException("Unable to load image at " + path);
    }

    if (m_imageMap.find(id) != m_imageMap.end()) {
        ostringstream errorStream;
        errorStream << "Image under id " << id << " already exists.";
        throw CapEngineException(errorStream.str());
    }

    Image image;
    image.path = path;
    image.texture = tempTexture;
    m_imageMap[id] = image;
}

void AssetManager::parseAssetFile(XmlParser& parser)
{
    // get Images nodes at /assets/images/image
    vector<XmlNode> images = parser.getNodes("/assets/textures/texture");
    auto imageIter = images.begin();
    for (; imageIter != images.end(); imageIter++) {
        string id = parser.getAttribute(*imageIter, "id");
        std::filesystem::path path = parser.getStringValue(*imageIter);
        string frameWidth = parser.getAttribute(*imageIter, "frameWidth");
        string frameHeight = parser.getAttribute(*imageIter, "frameHeight");
        string hasFrames = parser.getAttribute(*imageIter, "hasFrames");
        string isAnimation = parser.getAttribute(*imageIter, "isAnimation");

        // convert id to int
        istringstream idStream(id);
        int tId;
        idStream >> tId;

        auto getPath = [&](std::filesystem::path& in_path) -> std::filesystem::path {
            // convert path to absolute path.
            if (in_path.is_relative()) {
                return std::filesystem::path(*m_assetFile).parent_path() /= path;
            }
            return in_path;
        };

        if (isAnimation == "y") {
            int numFrames = std::stoi(parser.getAttribute(*imageIter, "numFrames"));
            int animationTimeMs = std::stoi(parser.getAttribute(*imageIter, "animationTimeMs"));
            m_animationMap[tId] = AnimatedImage{getPath(path).string(), nullptr, numFrames, animationTimeMs};
        }
        else {
            Image image;
            image.path = getPath(path).string();

            image.texture = nullptr;
            if (hasFrames == "y") {
                image.frames = parseFrames(parser, *imageIter);
            }

            m_imageMap[tId] = image;
        }
    }

    // get sounds at /assets/sounds
    vector<XmlNode> sounds = parser.getNodes("/assets/sounds/sound");
    auto soundIter = sounds.begin();
    for (; soundIter != sounds.end(); soundIter++) {
        string id = parser.getAttribute(*soundIter, "id");
        string path = parser.getStringValue(*soundIter);

        // convert id to int
        istringstream idStream(id);
        int sId;
        idStream >> sId;

        Sound sound;
        sound.path = path;
        sound.pcm = nullptr;

        m_soundMap[sId] = sound;
    }
}

Image* AssetManager::getImage(int id)
{
    // throw error if image has not been loaded
    auto iter = m_imageMap.find(id);
    if (iter == m_imageMap.end()) {
        throw AssetDoesNotExistError("image", id);
    }

    if (iter->second.texture == nullptr) {
        iter->second.texture = m_videoManager.loadImage(iter->second.path);
        if (iter->second.texture == nullptr) {
            throw CapEngineException("Unable to load image at " + iter->second.path);
        }
    }
    return &(iter->second);
}

std::optional<AnimatedImage> AssetManager::getAnimatedImage(int in_id)
{
    auto&& iter = m_animationMap.find(in_id);
    if (iter == m_animationMap.end()) {
        return std::nullopt;
    }

    if (iter->second.texture == nullptr) {
        iter->second.texture = Locator::videoManager->loadSharedImage(iter->second.path);
    }

    return iter->second;
}

SoftwareImage AssetManager::getSoftwareImage(int id)
{
    auto iter = m_imageMap.find(id);
    if (iter == m_imageMap.end()) {
        throw AssetDoesNotExistError("image", id);
    }

    Surface* surface = m_videoManager.loadSurface(iter->second.path);
    SoftwareImage softwareImage;
    softwareImage.path = iter->second.path;
    softwareImage.surface = surface;

    return softwareImage;
}

int AssetManager::getImageWidth(int id)
{
    Image* image = this->getImage(id);

    int width;
    width = m_videoManager.getTextureWidth(image->texture);
    return width;
}

int AssetManager::getImageHeight(int id)
{
    Image* image = this->getImage(id);

    int height;
    height = m_videoManager.getTextureHeight(image->texture);
    return height;
}

Frame AssetManager::getFrame(int assetID, std::string frameName)
{
    Image* pImage = this->getImage(assetID);
    assert(pImage != nullptr);

    auto frameIter = pImage->frames.find(frameName);
    if (frameIter == pImage->frames.end()) {
        std::ostringstream msg;
        msg << "Frame " << frameName << " does not exist";
        throw CapEngineException(msg.str());
    }

    return frameIter->second;
}

Frame AssetManager::getFrame(int assetID, int rowNum)
{
    Image* pImage = this->getImage(assetID);
    assert(pImage != nullptr);

    Frame frame;
    bool found = false;
    for (auto&& i : pImage->frames) {
        if (i.second.rowNum == rowNum) {
            frame = i.second;
            found = true;
        }
    }

    if (!found) {
        std::ostringstream msg;
        msg << "Frame " << rowNum << " does not exist";
        throw CapEngineException(msg.str());
    }

    return frame;
}

Sound* AssetManager::getSound(int id)
{
    // throw error if image has not been loaded
    auto iter = m_soundMap.find(id);
    if (iter == m_soundMap.end()) {
        throw AssetDoesNotExistError("sound", id);
    }

    if (iter->second.pcm == nullptr) {
        unique_ptr<PCM> upPcm(new PCM(iter->second.path));
        iter->second.pcm = upPcm.release();
        if (iter->second.pcm == nullptr) {
            throw CapEngineException("Unable to load image at " + iter->second.path);
        }
    }
    return &(iter->second);
}

void AssetManager::loadSound(int id, string path)
{
    unique_ptr<PCM> upTempPCM(new PCM(path));  // throws exception if failure

    if (m_soundMap.find(id) != m_soundMap.end()) {
        ostringstream errorStream;
        errorStream << "Sound under id " << id << " already exists.";
        throw CapEngineException(errorStream.str());
    }

    Sound sound;
    sound.path = path;
    sound.pcm = upTempPCM.release();
    m_soundMap[id] = sound;
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle _srcRect, Rectangle _destRect,
                        std::optional<double> rotationDegrees)
{
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

    m_videoManager.drawTexture(windowID, image->texture, &srcRect, &destRect, rotationDegrees);
}

void AssetManager::draw(Uint32 windowID, int id, Vector position)
{
    Image* image = this->getImage(id);

    Rect destRect;
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = m_videoManager.getTextureWidth(image->texture);
    destRect.h = m_videoManager.getTextureHeight(image->texture);

    m_videoManager.drawTexture(windowID, image->texture, nullptr, &destRect);
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle destRect)
{
    Image* image = this->getImage(id);
    Rect rect = destRect.toRect();
    m_videoManager.drawTexture(windowID, image->texture, nullptr, &rect);
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle _destRect, int row, int frameNum)
{
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

    m_videoManager.drawTexture(windowID, image->texture, &srcRect, &destRect);
}

int64_t AssetManager::playSound(int id, bool repeat)
{
    // TODO implement repeat functionality
    Sound* sound = getSound(id);
    unique_ptr<PCM> upTempPCM(new PCM(*(sound->pcm)));
    int64_t soundID = m_soundPlayer.addSound(upTempPCM.release(), repeat);
    return soundID;
}

void AssetManager::stopSound(int id) { m_soundPlayer.deleteSound(id); }

bool AssetManager::imageExists(int id) const { return m_imageMap.find(id) != m_imageMap.end(); }

bool AssetManager::soundExists(int id) const { return m_soundMap.find(id) != m_soundMap.end(); }


std::optional<std::filesystem::path> AssetManager::getBasePath() const
{
    return m_basePath;
}

}  // namespace CapEngine
