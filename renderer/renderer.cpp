#include "renderer.h"

namespace renderer
{
    Screen Renderer::Render(const Camera& camera, const World& world, Screen&& screen){
        screen.Clear();
        return screen;
    }
} // namespace renderer
