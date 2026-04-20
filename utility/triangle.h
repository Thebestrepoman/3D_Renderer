#pragma once

#include "algebra.h"
#include "vertex.h"

namespace renderer {
class Triangle {
public:
    Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    Triangle ChangeCoords(const Mat3& rmatrix, const Vec3& move) const;
    const Vertex& GetV1() const;
    const Vertex& GetV2() const;
    const Vertex& GetV3() const;
    void SetV1(const Vertex& v);
    void SetV2(const Vertex& v);
    void SetV3(const Vertex& v);
    void ApplyLight(const Light& light);
private:
    Vertex vertexes_[3];
};
}  // namespace renderer
