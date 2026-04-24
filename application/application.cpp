#include "application.h"

#include "start_params.h"

namespace renderer {
Application::Application()
    : world_(start_object, start_lights),
      camera_(start_focal_point, start_screen_angle_point, start_height_vector, start_width_vector, start_camera_height,
              start_camera_width, farsight, nearsight),
      screen_(start_width, start_height),
      runtime_(start_width, start_height),
      view_(runtime_.GetWindow(), start_width, start_height),
      eveproc_(),
      renderer_() {
}

void Application::Run() {
    while (runtime_.IsOpen()) {
        sf::Event event;
        while (runtime_.PollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                runtime_.Close();
            } else {
                eveproc_.HandleEvent(event, camera_);
            }
        }
        screen_ = std::move(renderer_.Render(camera_, world_, std::move(screen_)));
        view_.show(screen_);
    }
}
}  // namespace renderer
