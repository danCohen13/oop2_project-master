#pragma once
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

class RulesScreen : public Screen {
public:
    RulesScreen(ScreenStack& stack);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite m_backgroundSprite;

    std::unique_ptr<Button> m_backButton;
    std::unique_ptr<Button> m_nextButton;

    int m_currentPage;
    std::vector<std::string> m_pages;
    sf::Vector2f m_mousePos;
};