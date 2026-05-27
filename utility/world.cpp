#include "world.h"

namespace renderer {
World::World(std::vector<Object>&& objects, std::vector<Light>&& lights)
    : objects_(std::move(objects)), lights_(std::move(lights)) {
}

const std::vector<Object>& World::GetObjects() const {
    return objects_;
}

const std::vector<Light>& World::GetLights() const {
    return lights_;
}

void World::AddObject(const Object& object) {
    objects_.push_back(object);
}

void World::AddLight(const Light& light) {
    lights_.push_back(light);
}
}  // namespace renderer
