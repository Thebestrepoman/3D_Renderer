#pragma once

#include <Eigen/Dense>

namespace renderer {
using Vec3 = Eigen::Vector3d;
using Mat3 = Eigen::Matrix3d;

inline Vec3 CalculateOrientedNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
    return (v1 - v3).cross(v2 - v3).normalized();
}

inline Vec3 RotateVectorAroundAxis(const Vec3& v, const Vec3& axis, double angle) {
    return v * std::cos(angle) + axis.cross(v) * std::sin(angle) + axis * (axis.dot(v) * (1.0 - std::cos(angle)));
}
}  // namespace renderer
