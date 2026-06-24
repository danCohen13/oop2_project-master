#pragma once
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class GameOverScreen : public Screen {
public:
    GameOverScreen(ScreenStack& stack, int coins, int distance);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    sf::RectangleShape m_overlay;

    sf::ConvexShape m_panelBase;     
    sf::ConvexShape m_panelHighlight; 
    sf::ConvexShape m_panelBorder;    

    sf::Text m_gameOverText;
    sf::Text m_statsText;

    std::unique_ptr<Button> m_restartButton;
    std::unique_ptr<Button> m_menuButton;

    sf::Vector2f m_mousePos;
    int m_coins;
    int m_distance;
};