#include "colour.h"

namespace renderer {

Colour::Colour(double r, double g, double b) : r_(r), g_(g), b_(b){};

Colour Colour::Check() {
    double r = std::max(std::min(1.0, r_), 0.0);
    double g = std::max(std::min(1.0, g_), 0.0);
    double b = std::max(std::min(1.0, b_), 0.0);
    return {r, g, b};
}

Colour Colour::operator+(const Colour& other) const {
    return Colour(r_ + other.r_, g_ + other.g_, b_ + other.b_);
}

Colour Colour::operator-(const Colour& other) const {
    return Colour(r_ - other.r_, g_ - other.g_, b_ - other.b_);
}

Colour Colour::operator*(const double other) const {
    return Colour(r_ * other, g_ * other, b_ * other);
}

Colour& Colour::operator+=(const Colour& other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;
    return *this;
}

Colour& Colour::operator-=(const Colour& other) {
    r_ -= other.r_;
    g_ -= other.g_;
    b_ -= other.b_;
    return *this;
}

Colour& Colour::operator*=(const double other) {
    r_ *= other;
    g_ *= other;
    b_ *= other;
    return *this;
}

double Colour::GetR() const {
    return r_;
}

double Colour::GetG() const {
    return g_;
}

double Colour::GetB() const {
    return b_;
}

void Colour::SetR(double r) {
    r_ = r;
}

void Colour::SetG(double g) {
    g_ = g;
}

void Colour::SetB(double b) {
    b_ = b;
}

}  // namespace renderer
