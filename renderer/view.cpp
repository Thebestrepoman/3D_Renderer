#include "view.h"

#include <iostream>

namespace renderer {
View::View(sf::RenderWindow* window, ScreenWidth width, ScreenHeight height) : window_(window), image_() {
    image_.create(width, height);
}

void View::show(const Screen& screen) {
    window_->clear(sf::Color::Black);
    for (int i = 0; i < screen.Height(); i++) {
        for (int j = 0; j < screen.Width(); j++) {
            Colour cl = screen.GetPixel(j, i);
            SFMLColour pixel = screen.GetPixel(j, i);
            image_.setPixel(j, i, {pixel.GetR(), pixel.GetG(), pixel.GetB()});
        }
    }
    texture_.loadFromImage(image_);
    sprite_.setTexture(texture_);
    window_->draw(sprite_);
    window_->display();
}
}  // namespace renderer
