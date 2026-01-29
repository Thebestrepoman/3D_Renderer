#include "renderer.h"
#include <iostream>


namespace renderer {
ZBuffer::ZBuffer(int height, int width, const std::vector<Triangle>& triangles)
    : height_(height), width_(width), screen_(width * height, {0, 0, 0}), zBuffer_(width_ * height_, 1.0) {

    for (const Triangle& triangle : triangles) {
        Vec3 coord1 = triangle.GetV1().GetCoordinates();
        coord1.x() *= width_;
        coord1.y() *= height_;
        Vec3 coord2 = triangle.GetV2().GetCoordinates();
        coord2.x() *= width_;
        coord2.y() *= height_;
        Vec3 coord3 = triangle.GetV3().GetCoordinates();
        coord3.x() *= width_;
        coord3.y() *= height_;
        int minx = std::max(0.0, std::floor(std::min(coord1.x(), std::min(coord2.x(), coord3.x()))));
        int maxx = std::min(1.0 * (width_ - 1), std::ceil(std::max(coord1.x(), std::max(coord2.x(), coord3.x()))));
        int miny = std::max(0.0, std::floor(std::min(coord1.y(), std::min(coord2.y(), coord3.y()))));
        int maxy = std::min(1.0 * (height_ - 1), std::ceil(std::max(coord1.y(), std::max(coord2.y(), coord3.y()))));
        double square = (coord3 - coord1).cross(coord2 - coord1).z();
        if (abs(square) <= 1e-6) {
            continue;
        }
        for (int i = miny; i <= maxy; i++) {
            for (int j = minx; j <= maxx; j++) {
                Vec3 pixel(j + 0.5, i + 0.5, 0);
                double square1 = (pixel - coord2).cross(coord3 - coord2).z() / square;
                double square2 = (pixel - coord3).cross(coord1 - coord3).z() / square;
                double square3 = (pixel - coord1).cross(coord2 - coord1).z() / square;
                if ((square1 >= -1e-6) && (square2 >= -1e-6) && (square3 >= -1e-6)) {
                    double newz = coord1.z() * square1 + coord2.z() * square2 + coord3.z() * square3;
                    if (newz < zBuffer_[i * width_ + j]) {
                        zBuffer_[i * width_ + j] = coord1.z() * square1 + coord2.z() * square2 + coord3.z() * square3;
                        screen_[i * width_ + j] =
                            (triangle.GetV1().GetColour() * square1 + triangle.GetV2().GetColour() * square2 +
                             triangle.GetV3().GetColour() * square3)
                                .Check();
                    }
                }
            }
        }
    }
}

void ZBuffer::Draw(sf::RenderWindow& window) {
    sf::Image image;
    image.create(width_, height_);
    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            image.setPixel(j, i,
                           {static_cast<sf::Uint8>(screen_[i * width_ + j].GetR() * 255),
                            static_cast<sf::Uint8>(screen_[i * width_ + j].GetG() * 255),
                            static_cast<sf::Uint8>(screen_[i * width_ + j].GetB() * 255)});
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
}
}  // namespace renderer
