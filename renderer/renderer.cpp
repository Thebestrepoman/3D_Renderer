#include "renderer.h"
#include <iostream>

namespace renderer {
Screen Renderer::Render(const Camera& camera, const World& world, Screen&& screen) const {
    screen.Clear();
    std::vector<Triangle> clipped = std::move(ClipWorldWithCamera(camera, world));
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
            clipped = ClipTriangleWithPlanes(triangle.ChangeCoords(object.GetRmatrix(), object.GetMove()), std::move(clipped),
                                             camera_planes);
        }
    }
    std::cout << clipped.size();
    return clipped;
}
}  // namespace renderer
