#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// HUD en glassmorphism : petits panneaux en verre depoli derriere
// chaque groupe d'information (score+vies regroupes a gauche, distance a droite).
class HUD {
public:
    HUD();
    void updateTexts(int score, int lives, int distance);
    void draw(sf::RenderWindow& window) const;

private:
    // Construit un polygone aux coins arrondis (meme technique que Button)
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    sf::Font m_font;

    // --- Panneau gauche : Score + Vies regroupes ---
    sf::ConvexShape m_leftPanelBase;
    sf::ConvexShape m_leftPanelHighlight;
    sf::ConvexShape m_leftPanelBorder;
    sf::Text m_scoreText;
    sf::Text m_livesText;

    // --- Panneau droit : Distance ---
    sf::ConvexShape m_rightPanelBase;
    sf::ConvexShape m_rightPanelHighlight;
    sf::ConvexShape m_rightPanelBorder;
    sf::Text m_distanceText;
};