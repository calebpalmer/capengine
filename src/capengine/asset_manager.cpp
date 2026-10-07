// TODO This whole darn file needs refactoring
#include "asset_manager.h"
#include <SDL_surface.h>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>
#include <exception>
#include <filesystem>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include <jsoncons/json.hpp>

#include "CapEngineException.h"
#include "filesystem.h"
#include "jsoncons/json_exception.hpp"
#include "locator.h"
#include "xml_parser.h"
#include "logging.h"

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

/**
 * \brief Parse animation frames from a JSON array of frame objects.
 * \param json A JSON array where each element contains frameName, rowNum, frameWidth,
 *             frameHeight, numFrames, animationTime, and optionally horizontalPadding
 *             and verticalPadding.
 * \return Map of frame name to Frame, skipping duplicates with a warning.
 */
std::map<string, Frame> parseFrames(const jsoncons::json& json)
{
    std::map<string, Frame> frameMap;

    for (auto&& frame : json.array_range()) {
        try {
            const std::string frameName = frame["frameName"].as<std::string>();
            const int rowNum = frame["rowNum"].as<int>();
            const int frameWidth = frame["frameWidth"].as<int>();
            const int frameHeight = frame["frameHeight"].as<int>();
            const int numFrames = frame["numFrames"].as<int>();
            const double animationTime = frame["animationTime"].as<double>();
            const int horizontalPadding = frame.get_value_or<int>("horizontalPadding", 0);
            const int verticalPadding = frame.get_value_or<int>("verticalPadding", 0);

            if (frameMap.find(frameName) != frameMap.end()) {
                BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning)
                    << std::format("Frame with name {} already loaded for Texture.", frameName);
                continue;
            }

            frameMap.emplace(frameName, Frame{.frameName = frameName,
                                              .rowNum = rowNum,
                                              .frameWidth = frameWidth,
                                              .frameHeight = frameHeight,
                                              .numFrames = numFrames,
                                              .animationTime = animationTime,
                                              .horizontalPadding = horizontalPadding,
                                              .verticalPadding = verticalPadding});
        }
        catch (const std::exception& err) {
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::error)
                << "Error parsing frame:" << boost::diagnostic_information(err);
        }
    }

    return frameMap;
}

}  // end anonymous namespace

AssetManager::AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, const jsoncons::json& assetsJson,
                           std::filesystem::path in_basePath)
    : m_videoManager(videoManager), m_soundPlayer(soundPlayer), m_basePath(in_basePath)
{
    this->parseAssetFile(assetsJson);
}

AssetManager::AssetManager(VideoManager& videoManager, SoundPlayer& soundPlayer, std::optional<string> assetFile,
                           std::optional<std::filesystem::path> basePath)
    : m_videoManager(videoManager), m_soundPlayer(soundPlayer), m_assetFile(assetFile), m_basePath(std::move(basePath))
{
    // if the asset base path is not provided. try to find it.
    if (!m_basePath.has_value()) {
        // set it relative to the asset file if there is one.
        if (m_assetFile.has_value()) {
            m_basePath = std::filesystem::path{*m_assetFile}.parent_path();
        }

        // try to get it relative to the executable
        else {
            std::filesystem::path basePath =
                std::filesystem::path{CapEngine::getCurrentExecutablePath()}.parent_path().parent_path() / "resources";
            if (std::filesystem::exists(basePath) && std::filesystem::is_directory(basePath)) {
                m_basePath = basePath;
            }
        }
    }

    // make it absolute
    if (m_basePath && !m_basePath->is_absolute()) {
        m_basePath = std::filesystem::absolute(*m_basePath);
    }

    if (m_assetFile.has_value()) {
        m_assetFile = std::filesystem::absolute(*assetFile).string();

        bool parsed = false;
        // first try to parse it as json
        try {
            std::ifstream f{*assetFile, std::ios::in};
            if (!f) {
                CAP_THROW(CapEngineException{"Unable to open file " + *assetFile});
            }

            jsoncons::json assetsJson = jsoncons::json::parse(f);
            parseAssetFile(assetsJson);
            parsed = true;
        }
        catch (const jsoncons::ser_error& e) {
        }

        // after try to parse as xml
        if (!parsed) {
            try {
                XmlParser parser(*assetFile);
                parseAssetFile(parser);
                parsed = true;
            }
            catch (const CapEngineException& err) {
            }
        }
    }
}

AssetManager::AssetManager(std::optional<std::string> assetFile, std::optional<std::filesystem::path> in_basePath)
    : AssetManager::AssetManager(Locator::getVideoManager(), Locator::getSoundPlayer(), assetFile,
                                 std::move(in_basePath))
{
}

void AssetManager::loadImage(int id, string path, int frameWidth, int frameHeight)
{
    std::shared_ptr<Texture> texture = m_videoManager.loadSharedImage(path);
    if (texture == nullptr) {
        throw CapEngineException("Unable to load image at " + path);
    }

    if (m_imageMap.find(id) != m_imageMap.end()) {
        ostringstream errorStream;
        errorStream << "Image under id " << id << " already exists.";
        throw CapEngineException(errorStream.str());
    }

    Image image;
    image.path = path;
    image.texture = texture;
    m_imageMap[id] = image;
}

void AssetManager::loadSurface(int id, Surface* surface)
{
    CAP_THROW_NULL(surface);
    if (this->imageExists(id))
        CAP_THROW(CapEngineException{std::string{"Image with id "} + std::to_string(id) + " exists."});

    auto& videoManager = Locator::getVideoManager();
    auto staticTexture = videoManager.createTextureFromSurfacePtr(surface);

    // make a copy that has the flag SDL_TEXTUREACCESS_TARGET
    auto texture = videoManager.copyTexture(staticTexture.get());

    m_imageMap.emplace(id, Image{"", std::shared_ptr<Texture>(texture.release(), SDL_DestroyTexture)});
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

void AssetManager::parseAssetFile(const jsoncons::json& json)
{
    if (json.contains("textures")) {
        for (auto&& texture : json["textures"].array_range()) {
            try {
                const int id = texture["id"].as<int>();
                std::filesystem::path path = std::filesystem::path{texture["path"].as<std::string>()};
                if (path.is_relative())
                    path = *m_basePath / path;

                if (!std::filesystem::exists(path)) {
                    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning)
                        << std::format("{} does not exist", path.string());
                }

                const string frameWidth = texture["frameWidth"].as<std::string>();
                const string frameHeight = texture["frameHeight"].as<std::string>();
                const bool hasFrames = texture.get_value_or<bool>("hasFrames", false);
                const bool isAnimation = texture.get_value_or<bool>("isAnimation", false);

                // Is an AnimatedImage
                if (isAnimation) {
                    const int numFrames = texture["numFrames"].as<int>();
                    const int animationTimeMs = texture["animationTimeMs"].as<int>();
                    m_animationMap.emplace(id, AnimatedImage{.path = path,
                                                             .texture = nullptr,
                                                             .numFrames = numFrames,
                                                             .animationTimeMs = animationTimeMs});
                    continue;
                }

                std::map<std::string, Frame> frames{};
                if (hasFrames) {
                    frames = parseFrames(texture.at("frames"));
                }

                // Is a regular Image
                m_imageMap.emplace(id, Image{.path = path, .texture = nullptr, .frames = frames});
            }
            catch (const std::exception& err) {
                BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::error)
                    << "Error parsing texture:" << boost::diagnostic_information(err);
            }
        }
    }

    if (json.contains("sounds")) {
        for (auto&& sound : json["sounds"].array_range()) {
            try {
                const int id = sound["id"].as<int>();
                std::filesystem::path path{sound["path"].as<std::string>()};

                // if the path is relative make it absolute relative to base path
                if (m_basePath && !path.is_absolute()) {
                    path = *m_basePath / path;
                }

                if (!std::filesystem::exists(path)) {
                    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning)
                        << std::format("{} does not exist", path.string());
                }

                m_soundMap.emplace(id, Sound{.path = path, .pcm = nullptr});
            }
            catch (const std::exception& err) {
                BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::error)
                    << "Error parsing sound:" << boost::diagnostic_information(err);
            }
        }
    }
}

Image AssetManager::getImage(int id)
{
    // throw error if image has not been loaded
    auto iter = m_imageMap.find(id);
    if (iter == m_imageMap.end()) {
        throw AssetDoesNotExistError("image", id);
    }

    if (iter->second.texture == nullptr) {
        iter->second.texture = m_videoManager.loadSharedImage(iter->second.path);
        if (iter->second.texture == nullptr) {
            CAP_THROW(CapEngineException("Unable to load image at " + iter->second.path));
        }
    }
    return iter->second;
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

    SoftwareImage softwareImage;

    if (iter->second.texture != nullptr) {
        SurfacePtr surface = m_videoManager.createSurfaceFromTexture(iter->second.texture.get());
        softwareImage.surface = std::move(surface);
    }
    else {
        SurfacePtr surface = m_videoManager.loadSurfacePtr(iter->second.path);
        softwareImage.surface = std::move(surface);
    }
    softwareImage.path = iter->second.path;

    return softwareImage;
}

int AssetManager::getImageWidth(int id)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture);

    double width = m_videoManager.getTextureWidth(image.texture.get());
    // TODO this should probably return a double to prevent narrowing
    return boost::numeric_cast<int>(width);
}

int AssetManager::getImageHeight(int id)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture)

    double height = m_videoManager.getTextureHeight(image.texture.get());
    // TODO this should probably return a double to prevent narrowing
    return boost::numeric_cast<int>(height);
}

Frame AssetManager::getFrame(int assetID, std::string frameName)
{
    Image image = this->getImage(assetID);

    auto frameIter = image.frames.find(frameName);
    if (frameIter == image.frames.end()) {
        std::ostringstream msg;
        msg << "Frame " << frameName << " does not exist";
        throw CapEngineException(msg.str());
    }

    return frameIter->second;
}

Frame AssetManager::getFrame(int assetID, int rowNum)
{
    Image image = this->getImage(assetID);

    Frame frame;
    bool found = false;
    for (auto&& i : image.frames) {
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

Sound AssetManager::getSound(int id)
{
    // throw error if image has not been loaded
    auto iter = m_soundMap.find(id);
    if (iter == m_soundMap.end()) {
        throw AssetDoesNotExistError("sound", id);
    }

    if (iter->second.pcm == nullptr) {
        iter->second.pcm = std::make_shared<PCM>(iter->second.path);
        if (iter->second.pcm == nullptr) {
            throw CapEngineException("Unable to load sound at " + iter->second.path);
        }
    }
    return iter->second;
}

void AssetManager::loadSound(int id, string path)
{
    std::shared_ptr<PCM> pcm(new PCM(path));  // throws exception if failure

    if (m_soundMap.find(id) != m_soundMap.end()) {
        ostringstream errorStream;
        errorStream << "Sound under id " << id << " already exists.";
        throw CapEngineException(errorStream.str());
    }

    Sound sound;
    sound.path = path;
    sound.pcm = pcm;
    m_soundMap[id] = sound;
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle _srcRect, Rectangle _destRect,
                        std::optional<double> rotationDegrees)
{
    Image image = this->getImage(id);

    Rect srcRect = _srcRect.toRect();
    Rect destRect = _destRect.toRect();

    CAP_THROW_NULL(image.texture);
    m_videoManager.drawTexture(windowID, image.texture.get(), srcRect, destRect, rotationDegrees);
}

void AssetManager::draw(Uint32 windowID, int id, Vector position)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture);

    Rect destRect;
    destRect.x = boost::numeric_cast<int>(position.x);
    destRect.y = boost::numeric_cast<int>(position.y);
    destRect.w = m_videoManager.getTextureWidth(image.texture.get());
    destRect.h = m_videoManager.getTextureHeight(image.texture.get());

    m_videoManager.drawTexture(windowID, image.texture.get(), std::nullopt, destRect);
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle destRect)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture);
    Rect rect = destRect.toRect();
    m_videoManager.drawTexture(windowID, image.texture.get(), std::nullopt, rect);
}

void AssetManager::draw(Uint32 windowID, int id, Rectangle _destRect, int row, int frameNum)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture);

    Frame frame = this->getFrame(id, row);
    // need to add some checkingto make sure row and frames exists

    Rect srcRect;
    srcRect.x = frameNum * frame.frameWidth;
    srcRect.y = row * frame.frameHeight;
    srcRect.w = frame.frameWidth;
    srcRect.h = frame.frameHeight;

    Rect destRect = _destRect.toRect();

    m_videoManager.drawTexture(windowID, image.texture.get(), srcRect, destRect);
}

void AssetManager::drawFrame(Uint32 windowId, int id, std::string frameName, int frameNumber, Rectangle destRect)
{
    Image image = this->getImage(id);
    CAP_THROW_NULL(image.texture);

    Frame frame = this->getFrame(id, frameName);

    Rect srcRect;
    srcRect.x = frameNumber * frame.frameWidth;
    srcRect.y = frame.rowNum * frame.frameHeight;
    srcRect.w = frame.frameWidth;
    srcRect.h = frame.frameHeight;

    m_videoManager.drawTexture(windowId, image.texture.get(), srcRect, destRect.toRect());
}

int64_t AssetManager::playSound(int id, bool repeat)
{
    // TODO implement repeat functionality
    Sound sound = getSound(id);
    auto pcm = std::make_unique<PCM>(*sound.pcm);
    int64_t soundID = m_soundPlayer.addSound(std::move(pcm), repeat);
    return soundID;
}

void AssetManager::stopSound(int id)
{
    m_soundPlayer.deleteSound(id);
}

bool AssetManager::imageExists(int id) const
{
    return m_imageMap.find(id) != m_imageMap.end();
}

bool AssetManager::soundExists(int id) const
{
    return m_soundMap.find(id) != m_soundMap.end();
}

std::optional<std::filesystem::path> AssetManager::getBasePath() const
{
    return m_basePath;
}

}  // namespace CapEngine
