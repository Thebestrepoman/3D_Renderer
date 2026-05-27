#pragma once

#include "../renderer/camera.h"
#include "../renderer/runtime.h"
#include "../renderer/screen.h"
#include "../renderer/view.h"
#include "../utility/world.h"
#include "../renderer/renderer.h"
#include "eventprocessor.h"

namespace renderer {

class Application {
public:
    Application();
    void Run();

private:
    World world_;
    Camera camera_;
    Screen screen_;
    RunTime runtime_;
    View view_;
    EventProcessor eveproc_;
    Renderer renderer_;
};

}  // namespace renderer
