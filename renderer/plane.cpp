#include "plane.h"

#include "../application/start_params.h"

namespace renderer {
Plane::Plane(const Vec3& normal, const Vec3& point) : normal_(normal), d_(-normal.dot(point)) {
}

int Plane::CheckSideForPoint(const Vec3& point) const {
    return (normal_.dot(point) + d_) < varepsilon;
}

Vertex Plane::Intersect(const Vertex& v1, const Vertex& v2) const {
    double t =
        (normal_.dot(v1.GetCoordinates()) + d_) / (normal_.dot(v1.GetCoordinates()) - normal_.dot(v2.GetCoordinates()));
    return {v1.GetCoordinates() + t * (v2.GetCoordinates() - v1.GetCoordinates()),
            (v1.GetColour() + (v2.GetColour() - v1.GetColour()) * t).Check(),
            (v1.GetNormal() + t * (v2.GetNormal() - v1.GetNormal())).normalized()};
}

std::vector<Triangle> Plane::ClipTriangleWithPlane(const Triangle& triangle) const {
    int inside[3];
    inside[0] = CheckSideForPoint(triangle.GetV1().GetCoordinates());
    inside[1] = CheckSideForPoint(triangle.GetV2().GetCoordinates());
    inside[2] = CheckSideForPoint(triangle.GetV3().GetCoordinates());
    int sum = 0;
    sum = inside[0] + inside[1] + inside[2];
    std::vector<Triangle> clipped;
    if (sum == 1) {
        if (inside[0] == 1) {
            clipped.emplace_back(triangle.GetV1(), Intersect(triangle.GetV1(), triangle.GetV2()),
                                 Intersect(triangle.GetV1(), triangle.GetV3()));
        }
        if (inside[1] == 1) {
            clipped.emplace_back(triangle.GetV2(), Intersect(triangle.GetV2(), triangle.GetV1()),
                                 Intersect(triangle.GetV2(), triangle.GetV3()));
        }
        if (inside[2] == 1) {
            clipped.emplace_back(triangle.GetV3(), Intersect(triangle.GetV3(), triangle.GetV2()),
                                 Intersect(triangle.GetV3(), triangle.GetV1()));
        }
    }
    if (sum == 2) {
        if (inside[0] == 0) {
            Vertex v3 = Intersect(triangle.GetV2(), triangle.GetV1());
            Vertex v4 = Intersect(triangle.GetV3(), triangle.GetV1());
            clipped.emplace_back(triangle.GetV2(), v3, v4);
            clipped.emplace_back(triangle.GetV2(), triangle.GetV3(), v4);
        }
        if (inside[1] == 0) {
            Vertex v3 = Intersect(triangle.GetV1(), triangle.GetV2());
            Vertex v4 = Intersect(triangle.GetV3(), triangle.GetV2());
            clipped.emplace_back(triangle.GetV1(), v3, v4);
            clipped.emplace_back(triangle.GetV1(), triangle.GetV3(), v4);
        }
        if (inside[2] == 0) {
            Vertex v3 = Intersect(triangle.GetV1(), triangle.GetV3());
            Vertex v4 = Intersect(triangle.GetV2(), triangle.GetV3());
            clipped.emplace_back(triangle.GetV1(), v3, v4);
            clipped.emplace_back(triangle.GetV1(), triangle.GetV2(), v4);
        }
    }
    if (sum == 3) {
        clipped.push_back(triangle);
    }
    return clipped;
}
}  // namespace renderer
