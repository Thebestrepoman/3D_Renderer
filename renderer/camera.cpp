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

void Camera::Move(const Vec3& offset) {
    focal_point_ = focal_point_ + offset;
    screen_angle_point_ = screen_angle_point_ + offset;
}

void Camera::Rotate(double yaw_angle, double xaw_angle) {
    Vec3 yaw_axis = height_vector_; 
    if (std::abs(yaw_angle) > 1e-6) {
        forward_vector_ = RotateVectorAroundAxis(forward_vector_, yaw_axis, yaw_angle).normalized();
        width_vector_ = RotateVectorAroundAxis(width_vector_, yaw_axis, yaw_angle).normalized();
        height_vector_ = RotateVectorAroundAxis(height_vector_, yaw_axis, yaw_angle).normalized();
        Vec3 to_screen_angle = screen_angle_point_ - focal_point_;
        to_screen_angle = RotateVectorAroundAxis(to_screen_angle, yaw_axis, yaw_angle);
        screen_angle_point_ = focal_point_ + to_screen_angle;
    }
    Vec3 xaw_axis = width_vector_;
    if (std::abs(xaw_angle) > 1e-6) {
        forward_vector_ = RotateVectorAroundAxis(forward_vector_, xaw_axis, xaw_angle).normalized();
        height_vector_ = RotateVectorAroundAxis(height_vector_, xaw_axis, xaw_angle).normalized();
        Vec3 to_screen_angle = screen_angle_point_ - focal_point_;
        to_screen_angle = RotateVectorAroundAxis(to_screen_angle, xaw_axis, xaw_angle);
        screen_angle_point_ = focal_point_ + to_screen_angle;
    }
}

}  // namespace renderer
