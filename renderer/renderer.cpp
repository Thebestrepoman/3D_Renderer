#include "renderer.h"

#include <iostream>

#include "../application/start_params.h"

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
                                light_res += l.colour_ *
                                             std::max(0.0, light_dir.normalized().dot(triangle.GetVi(i).GetNormal())) *
                                             l.candella_ * (1.0 / distance_sq);
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

    if (z < varepsilon) {
        z = varepsilon;
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

int ClampToScreen(double value, int max_dimension) {
    return std::clamp(static_cast<int>(value), 0, max_dimension - 1);
}

double CalculateTriangleDoubleArea(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
    return (v3 - v1).cross(v2 - v1).z();
}

Screen Renderer::DrawProjectedOnScreen(Screen&& screen, std::vector<Triangle>&& projected) const {
    for (const Triangle& triangle : projected) {
        Vec3 c1 = triangle.GetV1Coords();
        c1.x() *= screen.Width();
        c1.y() *= screen.Height();
        Vec3 c2 = triangle.GetV2Coords();
        c2.x() *= screen.Width();
        c2.y() *= screen.Height();
        Vec3 c3 = triangle.GetV3Coords();
        c3.x() *= screen.Width();
        c3.y() *= screen.Height();
        int minx = ClampToScreen(std::floor(std::min({c1.x(), c2.x(), c3.x()})), screen.Width());
        int maxx = ClampToScreen(std::ceil(std::max({c1.x(), c2.x(), c3.x()})), screen.Width());
        int miny = ClampToScreen(std::floor(std::min({c1.y(), c2.y(), c3.y()})), screen.Height());
        int maxy = ClampToScreen(std::ceil(std::max({c1.y(), c2.y(), c3.y()})), screen.Height());
        double double_square = CalculateTriangleDoubleArea(c1, c2, c3);
        if (std::abs(double_square) <= varepsilon) {
            continue;
        }
        for (int i = miny; i <= maxy; i++) {
            for (int j = minx; j <= maxx; j++) {
                Vec3 pixel(j + 0.5, i + 0.5, 0);
                double w1 = (pixel - c2).cross(c3 - c2).z() / double_square;
                double w2 = (pixel - c3).cross(c1 - c3).z() / double_square;
                double w3 = (pixel - c1).cross(c2 - c1).z() / double_square;
                if (w1 < -varepsilon || w2 < -varepsilon || w3 < -varepsilon) {
                    continue;
                }
                double new_z = c1.z() * w1 + c2.z() * w2 + c3.z() * w3;
                if (new_z >= screen.GetZ(j, i)) {
                    continue;
                }
                screen.SetZ(j, i, new_z);

                auto interpolated_colour = triangle.GetV1().GetColour() * w1 + triangle.GetV2().GetColour() * w2 +
                                           triangle.GetV3().GetColour() * w3;
                screen.DrawPixel(j, i, interpolated_colour.Check());
            }
        }
    }
    return std::move(screen);
}
}  // namespace renderer
