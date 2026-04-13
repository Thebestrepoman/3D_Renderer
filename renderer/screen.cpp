#include "screen.h"

namespace renderer {
Screen::Screen(ScreenWidth width, ScreenHeight height)
    : screen_(height, std::vector<SFMLColour>(width, SFMLColour(0.0, 0.0, 0.0))) {
}

int Screen::Height() const {
    return screen_.size();
}

int Screen::Width() const {
    return screen_[0].size();
}

void Screen::DrawPixel(int x, int y, SFMLColour&& colour) {
    screen_[y][x] = std::move(colour);
}
}  // namespace renderer
