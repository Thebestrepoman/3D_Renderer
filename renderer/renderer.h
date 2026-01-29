#include "camera.h"

#include <SFML/Graphics.hpp>
namespace renderer {
class ZBuffer {
public:
    ZBuffer(int height, int width, const std::vector<Triangle>& triangles);
    void Draw(sf::RenderWindow& window);
private:
    int height_;
    int width_;
    std::vector<Colour> screen_;
    std::vector<double> zBuffer_;
};
}  // namespace renderer
