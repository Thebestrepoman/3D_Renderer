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

std::vector<Triangle> Camera::Clip(const World& world) {
    std::vector<Triangle> clipped;
    for (const Object& obj : world.GetObjects()) {
        for (const Triangle& tr : obj.GetTriangles()) {
            ClipTriangleWithCamera(tr.ChangeeCoords(obj.GetRmatrix(), obj.GetMove()), clipped);
        }
    }
    for (Triangle& tr : clipped) {
        for (const Light& light : world.GetLights()) {
            tr.ApplyLight(light);
        }
    }

    return clipped;
}

Vertex Camera::ProjectiveTransformationForVertex(const Vertex& vertex) {
    Vec3 to_point = vertex.GetCoordinates() - focal_point_;
    double x = to_point.dot(width_vector_);
    double y = to_point.dot(height_vector_);
    double z = to_point.dot(forward_vector_);
    if (z < 1e-5) {
        z = 1e-5;
    }
    x = (x / z + 1) / 2;
    y = (y / z + 1) / 2;
    z = z / farsight_;
    return Vertex({x, y, z}, vertex.GetColour(), vertex.GetNormal());
}

std::vector<Triangle> Camera::ProjectiveTransformationForTriangles(const std::vector<Triangle>& clipped_and_colored) {
    std::vector<Triangle> projected;
    for (const Triangle& triangle : clipped_and_colored) {
        projected.emplace_back(ProjectiveTransformationForVertex(triangle.GetV1()),
                               ProjectiveTransformationForVertex(triangle.GetV2()),
                               ProjectiveTransformationForVertex(triangle.GetV3()));
    }
    return projected;
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
