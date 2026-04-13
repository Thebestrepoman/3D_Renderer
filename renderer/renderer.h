#pragma once
#include <SFML/Graphics.hpp>

#include "camera.h"
#include "screen.h"
namespace renderer {
class Renderer {
public:
    Renderer() = default;
    Screen Render(const Camera& camera, const World& world, Screen&& screen) const;
private:
    std::vector<Triangle> ClipWorldWithCamera(const Camera& camera, const World& world) const;
    std::vector<Triangle> ApplyLightToTriangles(const Camera& camera, const World& world, std::vector<Triangle>&& clipped) const;
    std::vector<Triangle> ProjectTrianglesToPlane(const Camera& camera, std::vector<Triangle>&& enlightened) const;
    Screen DrawProjectedOnScreen(Screen&& screen, std::vector<Triangle>&& projected) const;
};

}  // namespace renderer
