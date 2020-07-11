#ifndef CAPENGINE_IMAGELAYER_H
#define CAPENGINE_IMAGELAYER_H

#include "VideoManager.h"
#include "captypes.h"
#include "collision.h"
#include "layer.h"
#include "layerfactory.h"
#include "scene2dschema.h"
#include "scene2dutils.h"

#include <string_view>

namespace CapEngine
{

//! Layer for display an image in the scene (background for example).
class ImageLayer : public Layer
{
public:
  ImageLayer(int in_assetId, Rectangle in_position);

  static void registerConstructor(LayerFactory &in_factory);

  void update(double in_ms) override {}
  void render(const Camera2d &in_camera, uint32_t in_windowId) override;

protected:
  int m_assetId;        //! The id of the image asset.
  Rectangle m_position; // The size and position of the image.
};

namespace detail
{

//! Creates an image layer from json.
/**
 \param in_json
   The json to create the layer from.
 \return
   The layer.
*/
template <class T>
std::unique_ptr<T> makeImageLayer(const jsoncons::json &in_json)
{
  try {
    const int assetId = in_json[Schema::Scene2d::kAssetId].as<int>();
    const Rectangle position =
        JSONUtils::readRectangle(in_json[Schema::Scene2d::kPosition]);
    return std::make_unique<T>(assetId, std::move(position));
  }

  catch (const std::exception &e) {
    throw LayerCreationError(Schema::Scene2d::kImageLayerType,
                             boost::diagnostic_information(e));
  }
}

} // namespace detail
} // namespace CapEngine

#endif // CAPENGINE_IMAGELAYER_H
