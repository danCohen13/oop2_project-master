#pragma once
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Ecran de fin de partie en glassmorphism :
// fond assombri + panneau central en verre depoli + boutons en verre.
class GameOverScreen : public Screen {
public:
    GameOverScreen(ScreenStack& stack, int coins, int distance);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    // Construit un polygone aux coins arrondis (meme technique que Button)
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    // --- Fond ---
    sf::RectangleShape m_overlay; // voile sombre derriere tout

    // --- Panneau central en verre depoli ---
    sf::ConvexShape m_panelBase;      // corps translucide du panneau
    sf::ConvexShape m_panelHighlight; // reflet en haut du panneau
    sf::ConvexShape m_panelBorder;    // contour fin lumineux

    // --- Textes ---
    sf::Text m_gameOverText;
    sf::Text m_statsText;

    // --- Boutons (deja en glassmorphism) ---
    std::unique_ptr<Button> m_restartButton;
    std::unique_ptr<Button> m_menuButton;

    // --- Etat ---
    sf::Vector2f m_mousePos;
    int m_coins;
    int m_distance;
};