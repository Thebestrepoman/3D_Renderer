#include "light.h"

namespace renderer
{
    Light Light::CreateAmbientLight(double candella, const Colour& colour){
        Light light;
        light.lightenum = LightEnum::Ambient;
        light.Ambient = {candella, colour};
        return light;
    }

    Light Light::CreatePointLight(double candella, const Colour& colou, const Vec3& coordinates){
        Light light;
        light.lightenum = LightEnum::Point;
        light.Point = {candella, colou, coordinates};
        return light;
    }

    Light Light::CreateDirectionalLight(double candella, const Colour& colou, const Vec3& direction){
        Light light;
        light.lightenum = LightEnum::Directional;
        light.Directional = {candella, colou, direction};
        return light;
    }
} // namespace renderer
