#pragma once
#include <vector>
#include "sfmlcolour.h"

enum ScreenWidth : int;
enum ScreenHeight : int;

namespace renderer {
class Screen {
public:
    Screen(ScreenWidth width, ScreenHeight height);
    int Height() const;
    int Width() const;
    void DrawPixel(int x, int y, SFMLColour&& colour);
    void Clear();
private:
    std::vector<std::vector<SFMLColour>> screen_;
};
}  // namespace renderer
