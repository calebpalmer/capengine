#ifndef CAPENGINE_PLACEHOLDERGRAPHICS_H
#define CAPENGINE_PLACEHOLDERGRAPHICS_H

#include "components.h"
#include "collision.h"
#include "colour.h"

namespace CapEngine {

class PlaceHolderGraphics : public GraphicsComponent {
public:
	PlaceHolderGraphics(int in_width, int in_height, Colour in_colour);
	
	~PlaceHolderGraphics() override = default;

	std::unique_ptr<Component> clone() const override;
	
	void update(GameObject* object, double timestep) override {};
	void render(GameObject* object, const Camera2d &in_camera, uint32_t in_windowId) override;

private:
	int m_width;  //<! The width.
	int m_height; //<! The height.
	Colour m_colour; //<! The colour of the placeholder.
};

}

#endif // CAPENGINE_PLACEHOLDERGRAPHICS_H
