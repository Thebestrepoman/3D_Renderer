#include "renderer.h"

#include <iostream>

namespace renderer {
Screen Renderer::Render(const Camera& camera, const World& world, Screen&& screen) const {
    screen.Clear();
    std::vector<Triangle> clipped = std::move(ClipWorldWithCamera(camera, world));
    std::vector<Triangle> enlightned = std::move(ApplyLightToTriangles(camera, world, std::move(clipped)));
    std::vector<Triangle> projected = std::move(ProjectTrianglesToPlane(camera, std::move(enlightned)));
    screen = std::move(Renderer::DrawProjectedOnScreen(std::move(screen), std::move(projected)));
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
    const Vec3& focal_point = camera.GetFocalPoint();
    const Vec3& width_vector = camera.GetWidthVector();
    const Vec3& height_vector = camera.GetHeightVector();
    const Vec3& forward_vector = camera.GetForwardVector();
    
    Vec3 to_point = vertex.GetCoordinates() - focal_point;
    
    double x = to_point.dot(width_vector);
    double y = to_point.dot(height_vector);
    double z = to_point.dot(forward_vector);
    
    if (z < 1e-5) {
        z = 1e-5;
    }
    double projected_x = x / z;
    double projected_y = y / z;
    double half_width = camera.GetWidth() / 2.0; 
    double half_height = camera.GetHeight() / 2.0; 
    x = (projected_x / half_width + 1.0) / 2.0;
    y = (projected_y / half_height + 1.0) / 2.0;
    z = z / camera.GetFarsight();
    return Vertex({x, y, z}, vertex.GetColour(), vertex.GetNormal());
}
std::vector<Triangle> Renderer::ProjectTrianglesToPlane(const Camera& camera,
                                                        std::vector<Triangle>&& enlightened) const {
    std::vector<Triangle> projected;
    for (const Triangle& triangle : enlightened) {
        projected.emplace_back(ProjectiveTransformationForVertex(camera, triangle.GetV1()),
                               ProjectiveTransformationForVertex(camera, triangle.GetV2()),
                               ProjectiveTransformationForVertex(camera, triangle.GetV3()));
    }
    return projected;
}

Screen Renderer::DrawProjectedOnScreen(Screen&& screen, std::vector<Triangle>&& projected) const {
    for (const Triangle& triangle : projected) {
        Vec3 coord1 = triangle.GetV1().GetCoordinates();
        coord1.x() *= screen.Width();
        coord1.y() *= screen.Height();
        Vec3 coord2 = triangle.GetV2().GetCoordinates();
        coord2.x() *= screen.Width();
        coord2.y() *= screen.Height();
        Vec3 coord3 = triangle.GetV3().GetCoordinates();
        coord3.x() *= screen.Width();
        coord3.y() *= screen.Height();
        int minx = std::max(0.0, std::floor(std::min(coord1.x(), std::min(coord2.x(), coord3.x()))));
        int maxx =
            std::min(1.0 * (screen.Width() - 1), std::ceil(std::max(coord1.x(), std::max(coord2.x(), coord3.x()))));
        int miny = std::max(0.0, std::floor(std::min(coord1.y(), std::min(coord2.y(), coord3.y()))));
        int maxy =
            std::min(1.0 * (screen.Height() - 1), std::ceil(std::max(coord1.y(), std::max(coord2.y(), coord3.y()))));
        double square = (coord3 - coord1).cross(coord2 - coord1).z();
        if (abs(square) <= 1e-6) {
            continue;
        }
        for (int i = miny; i <= maxy; i++) {
            for (int j = minx; j <= maxx; j++) {
                Vec3 pixel(j + 0.5, i + 0.5, 0);
                double square1 = (pixel - coord2).cross(coord3 - coord2).z() / square;
                double square2 = (pixel - coord3).cross(coord1 - coord3).z() / square;
                double square3 = (pixel - coord1).cross(coord2 - coord1).z() / square;
                if ((square1 >= -1e-6) && (square2 >= -1e-6) && (square3 >= -1e-6)) {
                    double newz = coord1.z() * square1 + coord2.z() * square2 + coord3.z() * square3;
                    if (newz < screen.GetZ(j, i)) {
                        screen.SetZ(j, i, coord1.z() * square1 + coord2.z() * square2 + coord3.z() * square3);
                        screen.DrawPixel(
                            j, i,
                            (triangle.GetV1().GetColour() * square1 + triangle.GetV2().GetColour() * square2 +
                             triangle.GetV3().GetColour() * square3)
                                .Check());
                    }
                }
            }
        }
    }
    return screen;
}
}  // namespace renderer
