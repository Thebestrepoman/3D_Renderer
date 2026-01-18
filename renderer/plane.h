#pragma once

#include "../utility/algebra.h"
#include "../utility/triangle.h"

#include <variant>
#include <optional>

namespace renderer {
class Plane {
public:
    Plane(const Vec3& normal, const Vec3& point);
    bool CheckSideForPoint(const Vec3& point) const;
    Vertex IntersectEdgeWithPlane(const Vertex& v1, const Vertex& v2) const;
    std::variant<std::nullopt_t, Triangle, std::pair<Triangle, Triangle>> ClipTriangleWithPlane(
        const Triangle& triangle) const;

private:
    Vec3 normal_;
    double d_;
};
}  // namespace renderer
