#pragma once

#include <SFML/Graphics.hpp>
#include "../renderer/camera.h"
#include "../renderer/runtime.h"
#include "../renderer/screen.h"
#include "../renderer/view.h"
#include "../utility/world.h"

namespace renderer {
class EventProcessor {
public:
    EventProcessor() = default;
    void HandleEvent(const sf::Event& event, Camera& camera);
};
}  // namespace renderer
