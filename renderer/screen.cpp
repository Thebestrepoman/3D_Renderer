#include "screen.h"

namespace renderer {
Screen::Screen(ScreenWidth width, ScreenHeight height)
    : screen_(height, std::vector<Colour>(width, Colour(0.0, 0.0, 0.0))),
              zbuffer_(height, std::vector<double>(width, 1.0)) {
}

int Screen::Height() const {
    return screen_.size();
}

int Screen::Width() const {
    return screen_[0].size();
}

void Screen::DrawPixel(int x, int y, Colour&& colour) {
    screen_[y][x] = std::move(colour);
}

const Colour& Screen::GetPixel(int x, int y) const{
    return screen_[y][x];
}

double Screen::GetZ(int x, int y) const{
    return zbuffer_[y][x];
}

void Screen::SetZ(int x, int y, double z) {
    zbuffer_[y][x] = z;
}

void Screen::Clear() {
    for (size_t i = 0; i < screen_.size(); i++) {
        for (size_t j = 0; j < screen_[0].size(); j++) {
            screen_[i][j] = {0, 0, 0};
            zbuffer_[i][j] = 1.0;
        }
    }
}
}  // namespace renderer
