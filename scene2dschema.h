#ifndef CAPENGINE_SCENESCHEMA_H
#define CAPENGINE_SCENESCHEMA_H

namespace CapEngine { namespace Schema { namespace Scene2d {

// general
extern const char* kPosition;
extern const char* kOrientation;
extern const char* kXCoord;
extern const char* kYCoord;
extern const char* kZCoord;
extern const char* kSize;
extern const char* kWidth;
extern const char* kHeight;
extern const char* kType;
extern const char* kSubType;
extern const char* kAssetId;

// scenes
extern const char* kScenes;
extern const char* kSceneId;

// layers
extern const char* kLayers;
extern const char* kLayerOrder;
extern const char* kLayerType;

// image layers
extern const char* kImageLayerType;
// kAssetId
// kPosition

// objects
extern const char* kObjects;
extern const char* kComponents;

}}} // CapEngine::Schema::Scene2d

#endif // CAPENGINE_SCENESCHEMA_H
