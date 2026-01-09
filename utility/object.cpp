#include "object.h"

namespace renderer {
Object::Object(const std::vector<Triangle>& triangles, const Mat4& mrmatrix)
    : triangles_(triangles), mrmatrix_(mrmatrix) {
}
const std::vector<Triangle>& Object::GetTriangles() const {
    return triangles_;
}
const Mat4& Object::GetMrmatrix() const {
    return mrmatrix_;
}

void Object::SetMatrix(const Mat4& mrmatrix) {
    mrmatrix_ = mrmatrix;
}
}  // namespace renderer
