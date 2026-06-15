#include "HUD.h"
#include <iostream>

// CORRECTION SFML 3 : L'ordre exact est (m_font, "") et non pas ("", m_font) !
HUD::HUD()
    : m_font(),
    m_scoreText(m_font, ""),
    m_livesText(m_font, ""),
    m_distanceText(m_font, "")
{
    // Chargement de la police depuis ton dossier resources
    if (!m_font.openFromFile("resources/New Athletic M54.ttf")) {
        std::cerr << "Erreur: Impossible de charger la police pour le HUD.\n";
    }

    // Configuration du texte du Score
    m_scoreText.setCharacterSize(30);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition({ 20.f, 20.f });

    // Configuration du texte des Vies
    m_livesText.setCharacterSize(30);
    m_livesText.setFillColor(sf::Color::Red);
    m_livesText.setPosition({ 20.f, 60.f });

    m_distanceText.setCharacterSize(30);
    m_distanceText.setFillColor(sf::Color::Yellow);
    m_distanceText.setPosition({ 1000.f, 20.f });

    // Remplissage initial des chaînes
    updateTexts(0, 1, 0);
}

void HUD::updateTexts(int score, int lives, int distance) {
    m_scoreText.setString("Score: " + std::to_string(score));
    m_livesText.setString("Vies: " + std::to_string(lives));
    m_distanceText.setString(std::to_string(distance) + " m");
}

void HUD::draw(sf::RenderWindow& window) const {
    window.draw(m_scoreText);
    window.draw(m_livesText);
    window.draw(m_distanceText);
}