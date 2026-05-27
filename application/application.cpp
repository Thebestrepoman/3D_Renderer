#include "application.h"

#include "start_params.h"

namespace renderer {
Application::Application()
    : world_(std::move(start_object), std::move(start_lights)),
      camera_(start_focal_point, start_screen_angle_point, start_height_vector, start_width_vector, start_camera_height,
              start_camera_width, farsight, nearsight),
      screen_(start_width, start_height),
      runtime_(start_width, start_height),
      view_(runtime_.GetWindow(), start_width, start_height),
      eveproc_(),
      renderer_() {
}

void Application::Run() {
    sf::Clock fps_clock;  // Таймер для отсчета одной секунды
    int frame_count = 0;  // Счетчик кадров

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
        frame_count++;
        if (fps_clock.getElapsedTime().asSeconds() >= 1.0f) {
            float fps = frame_count / fps_clock.restart().asSeconds();
            runtime_.SetTitle("Application | FPS: " + std::to_string(static_cast<int>(fps)));
            frame_count = 0;
        }
    }
}
}  // namespace renderer
