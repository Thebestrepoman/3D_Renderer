#include "vertex.h"

namespace renderer {
Vertex::Vertex(const Vec3& coordinates, const Colour& colour, const Vec3& normal)
    : coordinates_(coordinates), colour_(colour), normal_(normal) {
}

Vertex::Vertex(const Vertex& other) : coordinates_(other.coordinates_), colour_(other.colour_), normal_(other.normal_) {
}

Vertex Vertex::ChangeeCoords(const Mat3& rmatrix, const Vec3& move) const {
    return {rmatrix * coordinates_ + move, colour_, rmatrix * normal_};
}

const Vec3& Vertex::GetCoordinates() const {
    return coordinates_;
}

const Colour& Vertex::GetColour() const {
    return colour_;
}

const Vec3& Vertex::GetNormal() const {
    return normal_;
}

void Vertex::SetCoordinates(const Vec3& coordinates) {
    coordinates_ = coordinates;
}

void Vertex::SetColour(const Colour& colour) {
    colour_ = colour;
}

void Vertex::SetNormal(const Vec3& normal) {
    normal_ = normal;
}

void Vertex::ApplyLight(const Light& light) {
    if (light.lightenum == LightEnum::Ambient) {
        colour_ += light.Ambient.colour_ * light.Ambient.candella_;
    } else if (light.lightenum == LightEnum::Directional) {
        colour_ += light.Directional.colour_ *
                   (std::max(0.0, -light.Directional.direction_.normalized().dot(normal_))) *
                   light.Directional.candella_;
    } else if (light.lightenum == LightEnum::Point) {
        colour_ += light.Point.colour_ *
                   (std::max(0.0, (light.Point.coordinates_ - coordinates_).normalized().dot(normal_))) *
                   light.Point.candella_;
    }
    colour_.CheckAndModify();
}

}  // namespace renderer
