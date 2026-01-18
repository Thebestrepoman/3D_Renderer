#include "object.h"

namespace renderer {
Object::Object(const std::vector<Triangle>& triangles, const Mat3& rmatrix, const Vec3& move)
    : triangles_(triangles), rmatrix_(rmatrix), move_(move) {
}
const std::vector<Triangle>& Object::GetTriangles() const {
    return triangles_;
}
const Mat3& Object::GetRmatrix() const {
    return rmatrix_;
}

void Object::SetRmatrix(const Mat3& rmatrix) {
    rmatrix_ = rmatrix;
}

const Vec3& Object::GetMove() const {
    return move_;
}

void Object::SetMove(const Vec3& move) {
    move_ = move;
}
}  // namespace renderer
