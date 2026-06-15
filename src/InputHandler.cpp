#include  "InputHandler.h"

    bool InputHandler::isEscapePressed(const sf::Event& event) const {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            return keyPressed->code == sf::Keyboard::Key::Escape;
        }
        return false;
    }

    bool InputHandler::isThrustingActive() const {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    }
