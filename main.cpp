#include "renderer/renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(2560, 1600), "Software 3D Renderer");
    renderer::World world;
    world.AddObject(renderer::ExtractFromObj("../tests/a.obj"));
    world.AddLight(renderer::Light::CreatePointLight(0.8, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}));
    renderer::Camera camera({0.0, 0.0, -1.0}, {-1.0, 1.0, 0.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0}, 1.68, 2.56, 100.0);
    std::vector<renderer::Triangle> clipped = camera.Clip(world);
    std::vector<renderer::Triangle> projected = camera.ProjectiveTransformationForTriangles(clipped);
    renderer::ZBuffer zbuffer(1600, 2560, projected);
    zbuffer.Draw(window);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
    }
}
