#include "triangle.h"

namespace renderer {
Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) : vertexes_{v1, v2, v3} {
}

Triangle Triangle::ChangeeCoords(const Mat3& rmatrix, const Vec3& move) const {
    return {vertexes_[0].ChangeeCoords(rmatrix, move), vertexes_[1].ChangeeCoords(rmatrix, move),
            vertexes_[2].ChangeeCoords(rmatrix, move)};
}

const Vertex& Triangle::GetV1() const {
    return vertexes_[0];
}
const Vertex& Triangle::GetV2() const {
    return vertexes_[1];
}
const Vertex& Triangle::GetV3() const {
    return vertexes_[2];
}

void Triangle::SetV1(const Vertex& v) {
    vertexes_[0] = v;
}

void Triangle::SetV2(const Vertex& v) {
    vertexes_[1] = v;
}
void Triangle::SetV3(const Vertex& v) {
    vertexes_[2] = v;
}
}  // namespace renderer
