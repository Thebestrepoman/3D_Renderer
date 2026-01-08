#include "object.h"

namespace renderer {
Object::Object(std::vector<Triangle> triangles, Mat4 mrmatrix = Mat4::Identity())
    : triangles_(triangles), mrmatrix_(mrmatrix) {
}
const std::vector<Triangle>& Object::GetTriangles() const{
    return triangles_;
}
const Mat4& Object::GetMrmatrix() const{
    return mrmatrix_;
}
}  // namespace renderer
