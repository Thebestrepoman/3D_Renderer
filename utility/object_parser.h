#pragma once

#include "triangle.h"

namespace renderer {
std::vector<Triangle> ExtractFromObj(const std::string& filename);
}  // namespace renderer
