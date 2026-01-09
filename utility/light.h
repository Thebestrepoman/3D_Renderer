#pragma once

#include "colour.h"
#include "algebra.h"

namespace renderer {
enum struct LightEnum {
    Ambient,
    Point,
    Directional,
};

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

struct Light {
    LightEnum lightenum;
    AmbientLight Ambient;
    PointLight Point;
    Directionalight Directional;

    static Light CreateAmbientLight(double candella, const Colour& colour);
    static Light CreatePointLight(double candella, const Colour& colou, const Vec3& coordinates);
    static Light CreateDirectionalLight(double candella, const Colour& colou, const Vec3& direction);
};
}  // namespace renderer
