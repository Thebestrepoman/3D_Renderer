#include "../renderer/camera.h"
#include "../renderer/runtime.h"
#include "../renderer/screen.h"
#include "../renderer/view.h"
#include "../utility/world.h"
#include "../utility/object_parser.h"

namespace renderer {
constexpr ScreenHeight start_height = static_cast<ScreenHeight>(1600);
constexpr ScreenWidth start_width = static_cast<ScreenWidth>(2560);

const Vec3 start_focal_point = {0, 0, -1};
const Vec3 start_screen_angle_point = {-1, 1, 0};
const Vec3 start_height_vector = {0, -1, 0};
const Vec3 start_width_vector = {1, 0, 0};
const double start_camera_height = 1.68;
const double start_camera_width = 2.56;
const double farsight = 100.0;
const double nearsight = 1e-9;

std::vector<Object> start_object = {{ExtractFromObj("../tests/a.obj")}};
}  // namespace renderer
