#include "eventprocessor.h"

namespace renderer {
void EventProcessor::HandleEvent(const sf::Event& event, Camera& camera) {
    double speed = 0.05;
    double rotate_speed = 0.03;
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                camera.Move(speed * camera.GetForwardVector());
                break;
            case sf::Keyboard::S:
                camera.Move(-speed * camera.GetForwardVector());
                break;
            case sf::Keyboard::A:
                camera.Move(-speed * camera.GetWidthVector());
                break;
            case sf::Keyboard::D:
                camera.Move(camera.GetWidthVector() * speed);
                break;
            case sf::Keyboard::Q:
                camera.Move(camera.GetHeightVector() * speed);
                break;
            case sf::Keyboard::E:
                camera.Move(camera.GetHeightVector() * -speed);
                break;
            case sf::Keyboard::Left:
                camera.Rotate(rotate_speed, 0.0);
                break;
            case sf::Keyboard::Right:
                camera.Rotate(-rotate_speed, 0.0);
                break;
            case sf::Keyboard::Up:
                camera.Rotate(0.0, -rotate_speed);
                break;
            case sf::Keyboard::Down:
                camera.Rotate(0.0, rotate_speed);
                break;
            default:
                break;
        }
    }
}
}  // namespace renderer
