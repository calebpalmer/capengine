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
const char* kSizeWidth = "width";
const char* kSizeHeight = "height";
const char* kType = "type";
const char* kSubType = "subtype";

// scenes
const char* kScenes = "scenes";
const char* kSceneId = "id";

// layers
const char* kLayers = "layers";
const char* kLayerOrder = "order";
const char* kLayerType = "type";

// objects
const char* kObjects = "objects";
const char* kComponents = "components";

}}} // CapEngine::Schema::Scene2d

#endif // CAPENGINE_SCENESCHEMA_H
