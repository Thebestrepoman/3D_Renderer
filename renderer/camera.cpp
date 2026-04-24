#include "camera.h"

namespace renderer {
Camera::Camera(const Vec3& focal_point, const Vec3& screen_angle_point, const Vec3& height_vector,
               const Vec3& width_vector, double height, double width, double farsight, double nearsight)
    : focal_point_(focal_point),
      screen_angle_point_(screen_angle_point),
      height_vector_(height_vector.normalized()),
      width_vector_(width_vector.normalized()),
      forward_vector_(height_vector.normalized().cross(width_vector.normalized()).normalized()),
      height_(height),
      width_(width),
      farsight_(farsight),
      nearsight_(nearsight) {
}

const Vec3& Camera::GetFocalPoint() const {
    return focal_point_;
}
const Vec3& Camera::GetScreenAnglePoint() const {
    return screen_angle_point_;
}
const Vec3& Camera::GetWidthVector() const {
    return width_vector_;
}
const Vec3& Camera::GetHeightVector() const {
    return height_vector_;
}
Vec3 Camera::GetForwardVector() const {
    return height_vector_.normalized().cross(width_vector_.normalized()).normalized();
}
double Camera::GetHeight() const {
    return height_;
}
double Camera::GetWidth() const {
    return width_;
}
double Camera::GetFarsight() const{
    return farsight_;
}

}  // namespace renderer
