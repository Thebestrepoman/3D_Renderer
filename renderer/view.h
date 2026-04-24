#pragma once

#include <SFML/Graphics.hpp>

#include "screen.h"
namespace renderer {
class View {
public:
    View(sf::RenderWindow* window, ScreenWidth width, ScreenHeight height);
    void show(const Screen& screen);

private:
    sf::RenderWindow* window_;
    sf::Image image_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};
}  // namespace renderer
