#include "scene2dschema.h"

namespace CapEngine
{
namespace Schema
{

namespace Scene2d
{

// general
const char *kPosition = "position";
const char *kOrientation = "orientation";
const char *kXCoord = "x";
const char *kYCoord = "y";
const char *kZCoord = "z";
const char *kSize = "size";
const char *kWidth = "width";
const char *kHeight = "height";
const char *kType = "type";
const char *kSubType = "subtype";
const char *kAssetId = "asset_id";
const char *kMetadata = "metadata";

// scenes
const char *kScenes = "scenes";
const char *kSceneId = "id";

// layers
const char *kLayers = "layers";
const char *kLayerOrder = "order";
const char *kLayerType = "type";

// image layers
const char *kImageLayerType = "ImageLayer";
// kAssetId
// kPosition
const char *kBitmapCollisionLayer = "BitmapCollisionLayer";

// vector layer
const char *kVectorCollisionLayerType = "VectorCollisionLayer";

// objects
const char *kObjects = "objects";
const char *kComponents = "components";

} // namespace Scene2d

namespace Components
{

// box collider
const char *kBox = "box";

} // namespace Components

} // namespace Schema
} // namespace CapEngine
