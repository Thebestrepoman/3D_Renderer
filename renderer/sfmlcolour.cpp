#include "sfmlcolour.h"

namespace renderer {
SFMLColour::SFMLColour(const Colour& colour) {
    r_ = static_cast<sf::Uint8>(255 * colour.GetR());
    g_ = static_cast<sf::Uint8>(255 * colour.GetG());
    b_ = static_cast<sf::Uint8>(255 * colour.GetB());
}

SFMLColour::SFMLColour(float r, float g, float b) {
    r_ = static_cast<sf::Uint8>(255 * r);
    g_ = static_cast<sf::Uint8>(255 * g);
    b_ = static_cast<sf::Uint8>(255 * b);
}

sf::Uint8 SFMLColour::GetR() const {
    return r_;
}
sf::Uint8 SFMLColour::GetG() const {
    return g_;
}
sf::Uint8 SFMLColour::GetB() const {
    return b_;
}
}  // namespace renderer
