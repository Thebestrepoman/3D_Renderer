#include "camera.h"

namespace renderer {
Camera::Camera(const Vec3& focal_point, const Vec3& screen_angle_point, const Vec3& height_vector,
               const Vec3& width_vector, double height, double width, double farsight)
    : focal_point_(focal_point),
      screen_angle_point_(screen_angle_point),
      height_vector_(height_vector.normalized()),
      width_vector_(width_vector.normalized()),
      forward_vector_(height_vector.normalized().cross(width_vector.normalized()).normalized()),
      height_(height),
      width_(width),
      farsight_(farsight) {
    RecalculatePlanes();
}

void Camera::RecalculatePlanes() {
    planes_.clear();
    Vec3 v2 = screen_angle_point_ + width_ * width_vector_;
    Vec3 v3 = screen_angle_point_ + height_ * height_vector_;
    Vec3 v4 = screen_angle_point_ + height_ * height_vector_ + width_ * width_vector_;
    planes_.emplace_back((screen_angle_point_ - focal_point_).cross(v2 - focal_point_).normalized(), focal_point_);
    planes_.emplace_back((v2 - focal_point_).cross(v4 - focal_point_).normalized(), focal_point_);
    planes_.emplace_back((v4 - focal_point_).cross(v3 - focal_point_).normalized(), focal_point_);
    planes_.emplace_back((v3 - focal_point_).cross(screen_angle_point_ - focal_point_).normalized(), focal_point_);
}

void Camera::ClipTriangleWithCamera(const Triangle& triangle, std::vector<Triangle>& clipped, int plane_num) {
    if (plane_num == 4) {
        clipped.push_back(triangle);
    }
    std::variant<std::nullopt_t, Triangle, std::pair<Triangle, Triangle>> clip_res =
        planes_[plane_num].ClipTriangleWithPlane(triangle);
    if (std::holds_alternative<std::nullopt_t>(clip_res)) {
        return;
    }
    if (std::holds_alternative<Triangle>(clip_res)) {
        ClipTriangleWithCamera(std::get<Triangle>(clip_res), clipped, plane_num + 1);
    }
    if (std::holds_alternative<std::pair<Triangle, Triangle>>(clip_res)) {
        ClipTriangleWithCamera(std::get<std::pair<Triangle, Triangle>>(clip_res).first, clipped, plane_num + 1);
        ClipTriangleWithCamera(std::get<std::pair<Triangle, Triangle>>(clip_res).second, clipped, plane_num + 1);
    }
}

std::vector<Triangle> Camera::Clip(const World& world) {
    std::vector<Triangle> clipped;
    for (const Object& obj : world.GetObjects()) {
        for (const Triangle& tr : obj.GetTriangles()) {
            ClipTriangleWithCamera(tr.ChangeeCoords(obj.GetRmatrix(), obj.GetMove()), clipped);
        }
    }
    for (Triangle& tr : clipped) {
        for (const Light& light: world.GetLights()) {
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

}  // namespace renderer
