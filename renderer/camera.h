#pragma once

#include "../utility/algebra.h"
#include "../utility/world.h"
#include "plane.h"

namespace renderer {
class Camera {
public:
    Camera(const Vec3& focal_point, const Vec3& screen_angle_point, const Vec3& height_vector, const Vec3& width_vector,
           double height, double width);
    void RecalculatePlanes();
    void ClipTriangleWithCamera(const Triangle& triangle, std::vector<Triangle>& clipped, int plane_num = 0);
    std::vector<Triangle> Clip(const World& world);

private:
    Vec3 focal_point_;
    Vec3 screen_angle_point_;
    Vec3 width_vector_;
    Vec3 height_vector_;
    double height_;
    double width_;
    std::vector<Plane> planes_;
};
}  // namespace renderer