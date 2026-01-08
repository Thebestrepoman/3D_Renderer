#pragma once

#include <Eigen/Dense>

namespace renderer
{
    using Vec3 = Eigen::Vector3d;
    using Mat4 = Eigen::Matrix4d;

Vec3 CalculateNormalForThreeVectors(const Vec3& v1, const Vec3& v2, const Vec3& v3){
    return (v1 - v3).cross(v2 - v3).normalized();
}
} // namespace renderer