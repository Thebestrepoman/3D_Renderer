#pragma once

#include "colour.h"
#include "algebra.h"

#include <variant>

namespace renderer {

struct AmbientLight {
    double candella_;
    Colour colour_;
};

struct PointLight {
    double candella_;
    Colour colour_;
    Vec3 coordinates_;
};

struct Directionalight {
    double candella_;
    Colour colour_;
    Vec3 direction_;
};

using LightVariant = std::variant<AmbientLight, PointLight, Directionalight>;

struct Light {
    LightVariant data;

    static Light CreateAmbient(double candella, const Colour& colour);

    static Light CreatePoint(double candella, const Colour& colour, const Vec3& coordinates);

    static Light CreateDirectional(double candella, const Colour& colour, const Vec3& direction);
};
}  // namespace renderer
