#include "MenuScreen.h"
#include "GameplayScreen.h"

MenuScreen::MenuScreen(ScreenStack& stack) : Screen(stack) {}

void MenuScreen::handleEvent(const sf::Event& event) {
    // Check variant state for mouse clicks
    if (event.is<sf::Event::MouseButtonReleased>()) {
        m_stack.push(std::make_unique<GameplayScreen>(m_stack));
    }
}

void MenuScreen::update(float ) {}
void MenuScreen::draw(sf::RenderWindow& window) {}