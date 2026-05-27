#pragma once

#include "../renderer/camera.h"
#include "../renderer/runtime.h"
#include "../renderer/screen.h"
#include "../renderer/view.h"
#include "../utility/object_parser.h"
#include "../utility/world.h"

namespace renderer {
constexpr ScreenHeight start_height = static_cast<ScreenHeight>(800);
constexpr ScreenWidth start_width = static_cast<ScreenWidth>(1280);

const Vec3 start_focal_point = {0, 0, -2};
const Vec3 start_screen_angle_point = {-1.28, 0.8, -3};;
const Vec3 start_height_vector = {0, -1, 0};
const Vec3 start_width_vector = {1, 0, 0};
const double start_camera_height = 1.6;
const double start_camera_width = 2.56;
const double farsight = 100.0;
const double nearsight = 1e-9;

const double varepsilon = 1e-6;

inline std::vector<Object> start_object = {{std::move(ExtractFromObj("../tests/teapot.obj"))}};
inline std::vector<Light> start_lights = {{Light::CreateDirectionalLight(1.0, {1.0, 1.0, 1.0}, {-3.0, 0.5, 1.0})}};

}  // namespace renderer
