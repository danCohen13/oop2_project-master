#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD();
    void updateTexts(int score, int lives, int distance);
    void draw(sf::RenderWindow& window) const;

private:
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    sf::Font m_font;

    sf::ConvexShape m_leftPanelBase;
    sf::ConvexShape m_leftPanelHighlight;
    sf::ConvexShape m_leftPanelBorder;
    sf::Text m_scoreText;
    sf::Text m_livesText;

    sf::ConvexShape m_rightPanelBase;
    sf::ConvexShape m_rightPanelHighlight;
    sf::ConvexShape m_rightPanelBorder;
    sf::Text m_distanceText;
};