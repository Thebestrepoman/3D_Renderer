#include "renderer.h"

#include <iostream>

namespace renderer {
Screen Renderer::Render(const Camera& camera, const World& world, Screen&& screen) const {
    screen.Clear();
    std::vector<Triangle> clipped = std::move(ClipWorldWithCamera(camera, world));
    std::vector<Triangle> enlightned = std::move(ApplyLightToTriangles(camera, world, std::move(clipped)));
    std::vector<Triangle> projected = std::move(ProjectTrianglesToPlane(camera, std::move(enlightned))); 
    return screen;
}

std::vector<Plane> CalculatePlanes(const Camera& camera) {
    const Vec3& screen_angle_point = camera.GetScreenAnglePoint();
    double width = camera.GetWidth();
    double height = camera.GetHeight();
    const Vec3& width_vector = camera.GetWidthVector();
    const Vec3& height_vector = camera.GetHeightVector();
    const Vec3& focal_point = camera.GetFocalPoint();
    std::vector<Plane> planes;
    Vec3 v2 = screen_angle_point + width * width_vector;
    Vec3 v3 = screen_angle_point + height * height_vector;
    Vec3 v4 = screen_angle_point + height * height_vector + width * width_vector;
    planes.emplace_back((screen_angle_point - focal_point).cross(v2 - focal_point).normalized(), focal_point);
    planes.emplace_back((v2 - focal_point).cross(v4 - focal_point).normalized(), focal_point);
    planes.emplace_back((v4 - focal_point).cross(v3 - focal_point).normalized(), focal_point);
    planes.emplace_back((v3 - focal_point).cross(screen_angle_point - focal_point).normalized(), focal_point);
    return planes;
}

std::vector<Triangle> ClipTriangleWithPlanes(const Triangle& triangle, std::vector<Triangle>&& clipped,
                                             const std::vector<Plane>& planes, int cur_plane = 0) {
    if (cur_plane == 4) {
        clipped.push_back(triangle);
        return clipped;
    }
    std::vector<Triangle> clipped_triangle = std::move(planes[cur_plane].ClipTriangleWithPlane(triangle));
    if (clipped_triangle.size() == 0) {
        return clipped;
    }
    if (clipped_triangle.size() == 1) {
        clipped = std::move(ClipTriangleWithPlanes(clipped_triangle[0], std::move(clipped), planes, cur_plane + 1));
    }
    if (clipped_triangle.size() == 2) {
        clipped = std::move(ClipTriangleWithPlanes(clipped_triangle[0], std::move(clipped), planes, cur_plane + 1));
        clipped = std::move(ClipTriangleWithPlanes(clipped_triangle[1], std::move(clipped), planes, cur_plane + 1));
    }
    return clipped;
}

std::vector<Triangle> Renderer::ClipWorldWithCamera(const Camera& camera, const World& world) const {
    std::vector<Plane> camera_planes = CalculatePlanes(camera);
    std::vector<Triangle> clipped;
    for (const auto& object : world.GetObjects()) {
        for (const auto& triangle : object.GetTriangles()) {
            clipped = ClipTriangleWithPlanes(triangle.ChangeCoords(object.GetRmatrix(), object.GetMove()),
                                             std::move(clipped), camera_planes);
        }
    }
    return clipped;
}

std::vector<Triangle> Renderer::ApplyLightToTriangles(const Camera& camera, const World& world,
                                                      std::vector<Triangle>&& clipped) const {
    for (auto& triangle : clipped) {
        for (int i = 0; i < 3; i++) {
            Colour light_res = {0, 0, 0};
            for (const auto& light : world.GetLights()) {
                std::visit(
                    [&](const auto& l) {
                        using T = std::decay_t<decltype(l)>;
                        if constexpr (std::is_same_v<T, AmbientLight>) {
                            light_res += l.colour_ * l.candella_;
                        } else if constexpr (std::is_same_v<T, Directionalight>) {
                            light_res += l.colour_ *
                                         std::max(0.0, -l.direction_.normalized().dot(triangle.GetVi(i).GetNormal())) *
                                         l.candella_;
                        } else if constexpr (std::is_same_v<T, PointLight>) {
                            Vec3 light_dir = l.coordinates_ - triangle.GetVi(i).GetCoordinates();
                            double distance_sq = light_dir.dot(light_dir);
                            if (distance_sq > 1e-6) {
                                double attenuation = 1.0 / distance_sq;
                                light_res += l.colour_ *
                                             std::max(0.0, light_dir.normalized().dot(triangle.GetVi(i).GetNormal())) *
                                             l.candella_ * attenuation;
                            }
                        }
                    },
                    light.data);
            }
            triangle.ChangeColourForVertex(light_res, i);
        }
    }
    return clipped;
}

Vertex ProjectiveTransformationForVertex(const Camera& camera, const Vertex& vertex) {
    const Vec3& screen_angle_point = camera.GetScreenAnglePoint();
    double width = camera.GetWidth();
    double height = camera.GetHeight();
    const Vec3& width_vector = camera.GetWidthVector();
    const Vec3& height_vector = camera.GetHeightVector();
    const Vec3& forward_vector = camera.GetForwardVector();
    const Vec3& focal_point = camera.GetFocalPoint();
    Vec3 to_point = vertex.GetCoordinates() - focal_point;
    double x = to_point.dot(width_vector);
    double y = to_point.dot(height_vector);
    double z = to_point.dot(forward_vector);
    if (z < 1e-5) {
        z = 1e-5;
    }
    x = (x / z + 1) / 2;
    y = (y / z + 1) / 2;
    z = z / camera.GetFarsight();
    return Vertex({x, y, z}, vertex.GetColour(), vertex.GetNormal());
}

std::vector<Triangle> Renderer::ProjectTrianglesToPlane(const Camera& camera, std::vector<Triangle>&& enlightened) const {
    std::vector<Triangle> projected;
    for (const Triangle& triangle : enlightened) {
        projected.emplace_back(ProjectiveTransformationForVertex(camera, triangle.GetV1()),
                               ProjectiveTransformationForVertex(camera, triangle.GetV2()),
                               ProjectiveTransformationForVertex(camera, triangle.GetV3()));
    }
    return projected;
}
}  // namespace renderer
