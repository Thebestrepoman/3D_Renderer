#include "view.h"

namespace renderer {
View::View(sf::RenderWindow* window) : window_(window) {
}

void View::show(const Screen& screen){
    window_->clear(sf::Color::Black);
}
}  // namespace renderer
