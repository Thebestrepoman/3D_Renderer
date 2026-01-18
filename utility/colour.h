#pragma once

#include <algorithm>

namespace renderer {
class Colour {
public:
    Colour() = default;
    Colour(double r, double g, double b);
    Colour Check();
    Colour operator+(const Colour& other) const;
    Colour operator-(const Colour& other) const;
    Colour operator*(const double other) const;
    Colour& operator+=(const Colour& other);
    Colour& operator-=(const Colour& other);
    Colour& operator*=(const double other);
    double GetR() const;
    double GetG() const;
    double GetB() const;
    void SetR(double r);
    void SetG(double g);
    void SetB(double b);

private:
    double r_;
    double g_;
    double b_;
};
}  // namespace renderer
