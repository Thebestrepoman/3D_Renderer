#pragma once

#include "triangle.h"

#include <string>
#include <fstream>
#include <sstream>

namespace renderer {
std::vector<Triangle> ExtractFromObj(const std::string& filename);
}  // namespace renderer
