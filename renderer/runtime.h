#pragma once
#include <SFML/Graphics.hpp>

#include "screen.h"
namespace renderer {
class RunTime {
public:
    RunTime(ScreenWidth screen_width, ScreenHeight screen_height);
    bool IsOpen();
    bool PollEvent(sf::Event& event);
    void Close();
    sf::RenderWindow* GetWindow();

private:
    sf::RenderWindow window_;
};

}  // namespace renderer
