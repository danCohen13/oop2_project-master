#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class InputHandler {
public:
    InputHandler() = default;
    ~InputHandler() = default;

    bool isEscapePressed(const sf::Event& event) const;
    bool isThrustingActive() const;
};