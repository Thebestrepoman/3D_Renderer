#include "light.h"

namespace renderer {
Light Light::CreateAmbientLight(double candella, const Colour& colour) {
    return Light{AmbientLight{candella, colour}};
}

Light Light::CreatePointLight(double candella, const Colour& colour, const Vec3& coordinates) {
    return Light{PointLight{candella, colour, coordinates}};
}

Light Light::CreateDirectionalLight(double candella, const Colour& colour, const Vec3& direction) {
    return Light{Directionalight{candella, colour, direction}};
}
}  // namespace renderer