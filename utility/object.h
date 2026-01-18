#pragma once

#include "algebra.h"
#include "triangle.h"
#include "object_parser.h"

#include <vector>
#include <string>

namespace renderer {

class Object {
public:
    Object() = default;
    Object(const std::vector<Triangle>& triangles, const Mat3& mrmatrix = Mat3::Identity(), const Vec3& move = Vec3::Zero());
    const std::vector<Triangle>& GetTriangles() const;
    const Mat3& GetRmatrix() const;
    void SetRmatrix(const Mat3& mrmatrix);
    const Vec3& GetMove() const;
    void SetMove(const Vec3& move);

private:
    std::vector<Triangle> triangles_;
    Mat3 rmatrix_;
    Vec3 move_;
};
}  // namespace renderer
