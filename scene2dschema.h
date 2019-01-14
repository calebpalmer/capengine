#ifndef CAPENGINE_SCENESCHEMA_H
#define CAPENGINE_SCENESCHEMA_H

namespace CapEngine { namespace Schema { namespace Scene2d {

// general
const char* kPosition = "position";
const char* kOrientation = "orientation";
const char* kXCoord = "x";
const char* kYCoord = "y";
const char* kZCoord = "z";
const char* kSize = "size";
const char* kWidth = "width";
const char* kHeight = "height";
const char* kType = "type";
const char* kSubType = "subtype";
const char* kAssetId = "asset_id";

// scenes
const char* kScenes = "scenes";
const char* kSceneId = "id";

// layers
const char* kLayers = "layers";
const char* kLayerOrder = "order";
const char* kLayerType = "type";

// image layers
const char* kImageLayerType = "imagelayer";
// kAssetId
// kPosition

// objects
const char* kObjects = "objects";
const char* kComponents = "components";

}}} // CapEngine::Schema::Scene2d

#endif // CAPENGINE_SCENESCHEMA_H
