#include "application.h"

#include "start_params.h"

namespace renderer {
Application::Application()
    : camera_(start_focal_point, start_screen_angle_point, start_height_vector, start_width_vector, start_camera_height,
              start_camera_width, farsight),
      screen_(start_width, start_height),
      runtime_(start_width, start_height),
      view_(runtime_.GetWindow()) {
}
}  // namespace renderer
