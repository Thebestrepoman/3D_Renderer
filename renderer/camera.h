#pragma once

#include "../utility/algebra.h"
#include "../utility/world.h"
#include "plane.h"

namespace renderer {
class Camera {
public:
    Camera(const Vec3& focal_point, const Vec3& screen_angle_point, const Vec3& height_vector, const Vec3& width_vector,
           double height, double width, double farsight, double nearsight);
    const Vec3& GetFocalPoint() const;
    const Vec3& GetScreenAnglePoint() const;
    const Vec3& GetWidthVector() const;
    const Vec3& GetHeightVector() const;
    Vec3 GetForwardVector() const;
    double GetHeight() const;
    double GetWidth() const;
    double GetFarsight() const;
    void Move(const Vec3& offset);
    void Rotate(double x_angle, double y_angle);

private:
    Vec3 focal_point_;
    Vec3 screen_angle_point_;
    Vec3 width_vector_;
    Vec3 height_vector_;
    Vec3 forward_vector_;
    double height_;
    double width_;
    std::vector<Plane> planes_;
    double farsight_;
    double nearsight_;
};
}  // namespace renderer
