#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD();
    void updateTexts(int score, int lives, int distance);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Font m_font;
    sf::Text m_scoreText;
    sf::Text m_livesText;
    sf::Text m_distanceText;
};