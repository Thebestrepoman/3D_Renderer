#include "plane.h"

namespace renderer {
Plane::Plane(const Vec3& normal, const Vec3& point) : normal_(normal), d_(-normal.dot(point)) {
}

bool Plane::CheckSideForPoint(const Vec3& point) const {
    return (normal_.dot(point) + d_) > 0.00001;
}

Vertex Plane::IntersectEdgeWithPlane(const Vertex& v1, const Vertex& v2) const {
    double t =
        (normal_.dot(v1.GetCoordinates()) + d_) / (normal_.dot(v1.GetCoordinates()) - normal_.dot(v2.GetCoordinates()));
    return {v1.GetCoordinates() + t * (v2.GetCoordinates() - v1.GetCoordinates()),
            (v1.GetColour() + (v2.GetColour() - v1.GetColour()) * t).Check(),
            (v1.GetNormal() + t * (v2.GetNormal() - v1.GetNormal())).normalized()};
}

std::variant<std::nullopt_t, Triangle, std::pair<Triangle, Triangle>> Plane::ClipTriangleWithPlane(
    const Triangle& triangle) const {
    bool inside[3];
    inside[0] = CheckSideForPoint(triangle.GetV1().GetCoordinates());
    inside[1] = CheckSideForPoint(triangle.GetV2().GetCoordinates());
    inside[2] = CheckSideForPoint(triangle.GetV3().GetCoordinates());
    int sum = 0;
    sum = inside[0] + inside[1] + inside[2];
    if (sum == 0) {
        return std::nullopt;
    }
    if (sum == 1) {
        if (inside[0] == 1) {
            return Triangle(triangle.GetV1(), IntersectEdgeWithPlane(triangle.GetV1(), triangle.GetV2()),
                            IntersectEdgeWithPlane(triangle.GetV1(), triangle.GetV3()));
        }
        if (inside[1] == 1) {
            return Triangle(triangle.GetV2(), IntersectEdgeWithPlane(triangle.GetV2(), triangle.GetV1()),
                            IntersectEdgeWithPlane(triangle.GetV2(), triangle.GetV3()));
        }
        if (inside[2] == 1) {
            return Triangle(triangle.GetV3(), IntersectEdgeWithPlane(triangle.GetV3(), triangle.GetV2()),
                            IntersectEdgeWithPlane(triangle.GetV3(), triangle.GetV1()));
        }
    }
    if (sum == 2) {
        if (inside[0] == 0) {
            Vertex v3 = IntersectEdgeWithPlane(triangle.GetV2(), triangle.GetV1());
            Vertex v4 = IntersectEdgeWithPlane(triangle.GetV3(), triangle.GetV1());
            return std::pair<Triangle, Triangle>(Triangle(triangle.GetV2(), v3, v4), Triangle(triangle.GetV2(), triangle.GetV3(), v4));
        }
        if (inside[1] == 0) {
            Vertex v3 = IntersectEdgeWithPlane(triangle.GetV1(), triangle.GetV2());
            Vertex v4 = IntersectEdgeWithPlane(triangle.GetV3(), triangle.GetV2());
            return std::pair<Triangle, Triangle>(Triangle(triangle.GetV1(), v3, v4), Triangle(triangle.GetV1(), triangle.GetV3(), v4));
        }
        if (inside[2] == 0) {
            Vertex v3 = IntersectEdgeWithPlane(triangle.GetV1(), triangle.GetV3());
            Vertex v4 = IntersectEdgeWithPlane(triangle.GetV2(), triangle.GetV3());
            return std::pair<Triangle, Triangle>(Triangle(triangle.GetV1(), v3, v4), Triangle(triangle.GetV1(), triangle.GetV2(), v4));
        }
    }
    return triangle;
}
}  // namespace renderer
