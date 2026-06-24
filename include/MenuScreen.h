#pragma once
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>

class MenuScreen : public Screen {
public:
    MenuScreen(ScreenStack& stack);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite m_backgroundSprite;
    sf::Sprite m_titleSprite; 

    std::unique_ptr<Button> m_playButton;
    std::unique_ptr<Button> m_rulesButton;
    std::unique_ptr<Button> m_highScoreButton;

    sf::Vector2f m_mousePos;
};