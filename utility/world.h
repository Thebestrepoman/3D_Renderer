#pragma once

#include "object.h"
#include "light.h"

namespace renderer
{
    class World {
    public:
        World(const std::vector<Object>& objects, const std::vector<Light>& lights);
        const std::vector<Object>& GetObjects();
        const std::vector<Light>& GetLights();
        void AddObject(const Object& object);
        void AddLight(const Light& light);
    private:
        std::vector<Object> objects_;
        std::vector<Light> lights_;
    };
} // namespace renderer
