#ifndef CAPENGINE_IMAGELAYER_H
#define CAPENGINE_IMAGELAYER_H

#include "layer.h"
#include "VideoManager.h"
#include "captypes.h"
#include "collision.h"

#include <string_view>

namespace CapEngine {

  //! Layer for display an image in the scene (background for example).
  class ImageLayer : public Layer {
  public:
    ImageLayer(int in_assetId, Rectangle in_position);

    static void registerConstructor();
    
    virtual void update(double in_ms) override {}
    virtual void render(const Camera2d &in_camera, uint32_t in_windowId) override;

  private:
    int m_assetId; //! The id of the image asset.
    Rectangle m_position; // The size and position of the image.
  };

} // namespace CapEngine

#endif // CAPENGINE_IMAGELAYER_H
