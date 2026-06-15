#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <memory>
#include "Screen.h"

class Controller {
public:
    Controller();
    ~Controller() = default;

    // Main application execution loop
    void run();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Central stack managing application screen states
    std::stack<std::unique_ptr<Screen>> m_screens;
};