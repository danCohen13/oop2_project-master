#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include <memory>

// Forward declaration of the Screen stack manager type
class Screen;
using ScreenStack = std::stack<std::unique_ptr<Screen>>;

class Screen {
public:
    Screen(ScreenStack& stack) : m_stack(stack) {}
    virtual ~Screen() = default;

    // Core lifecycle methods for every screen
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

protected:
    ScreenStack& m_stack; // Reference to change screens from within
};