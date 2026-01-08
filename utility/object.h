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
    Object(std::vector<Triangle> triangles, Mat4 mrmatrix = Mat4::Identity());
    Object(const std::string& filename, Mat4 mrmatrix = Mat4::Identity());
    const std::vector<Triangle>& GetTriangles() const;
    const Mat4& GetMrmatrix() const;

private:
    std::vector<Triangle> triangles_;
    Mat4 mrmatrix_;
};
}  // namespace renderer
