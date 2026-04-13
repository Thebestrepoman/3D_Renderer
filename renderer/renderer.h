#include "camera.h"
#include "screen.h"

#include <SFML/Graphics.hpp>
namespace renderer {
class Renderer {
public:
    Renderer() = default;
    Screen Render(const Camera& camera, const World& world, Screen screen);
};
}  // namespace renderer
