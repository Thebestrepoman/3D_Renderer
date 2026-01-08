#pragma once

#include "algebra.h"
#include "colour.h"
namespace renderer {
inline const Colour default_colour_k = Colour(0.2, 0.2, 0.2);

class Vertex {
public:
    Vertex(const Vec3& coordinates, const Colour& colour, const Vec3& normal);
    Vertex(const Vertex& other);
    const Vec3& GetCoordinates() const;
    const Colour& GetColour() const;
    const Vec3& GetNormal() const;
    void SetCoordinates(const Vec3& coordinates);
    void SetColour(const Colour& colour);
    void SetNormal(const Vec3& normal);
private:
    Vec3 coordinates_;
    Colour colour_;
    Vec3 normal_;
};

}  // namespace renderer
