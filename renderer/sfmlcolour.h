#pragma once
#include <SFML/Graphics.hpp>
#include "../utility/colour.h"

namespace renderer {
class SFMLColour {
public:
    SFMLColour(const Colour& colour);
    SFMLColour(float r, float g, float b);
    sf::Uint8 GetR() const;
    sf::Uint8 GetG() const;
    sf::Uint8 GetB() const;
private:
    sf::Uint8 r_;
    sf::Uint8 g_;
    sf::Uint8 b_;
};
}  // namespace renderer
