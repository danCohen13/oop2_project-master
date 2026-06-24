#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stack>
#include <memory>
#include "Screen.h"

class Controller {
public:
    Controller();
    ~Controller() = default;

    void run();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    std::stack<std::unique_ptr<Screen>> m_screens;
};