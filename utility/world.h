#pragma once

#include "object.h"
#include "light.h"

namespace renderer {
class World {
public:
    World() = default;
    World(const std::vector<Object>& objects, const std::vector<Light>& lights);
    const std::vector<Object>& GetObjects() const;
    const std::vector<Light>& GetLights() const;
    void AddObject(const Object& object);
    void AddLight(const Light& light);

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
};
}  // namespace renderer
