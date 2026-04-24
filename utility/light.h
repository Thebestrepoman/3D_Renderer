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

    static Light CreateAmbientLight(double candella, const Colour& colour);

    static Light CreatePointLight(double candella, const Colour& colour, const Vec3& coordinates);

    static Light CreateDirectionalLight(double candella, const Colour& colour, const Vec3& direction);
};
}  // namespace renderer
