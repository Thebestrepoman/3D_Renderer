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
    void DrawPixel(int x, int y, Colour&& colour);
    const Colour& GetPixel(int x, int y) const;
    void Clear();
    double GetZ(int x, int y) const;
    void SetZ(int x, int y, double z);
private:
    std::vector<std::vector<Colour>> screen_;
    std::vector<std::vector<double>> zbuffer_;
};
}  // namespace renderer
