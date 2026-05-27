#include "light.h"

namespace renderer {
Light Light::CreateAmbient(double candella, const Colour& colour) {
    return Light{AmbientLight{candella, colour}};
}

Light Light::CreatePoint(double candella, const Colour& colour, const Vec3& coordinates) {
    return Light{PointLight{candella, colour, coordinates}};
}

Light Light::CreateDirectional(double candella, const Colour& colour, const Vec3& direction) {
    return Light{Directionalight{candella, colour, direction}};
}
}  // namespace renderer
