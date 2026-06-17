#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Bouton style "glassmorphism" : fond semi-transparent flou-like,
// coins arrondis, bordure fine lumineuse, reflet en haut, et animation
// douce au survol (scale + eclaircissement).
class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size,
        const std::string& textStr, const sf::Font& font);

    void update(const sf::Vector2f& mousePos, float deltaTime);
    void draw(sf::RenderWindow& window) const;

    bool isClicked(const sf::Vector2f& mousePos) const;
    void setPosition(const sf::Vector2f& position);
    sf::FloatRect getGlobalBounds() const;

private:
    // Construit un polygone aux coins arrondis (approximation par arcs de cercle)
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    // --- Formes du verre ---
    sf::ConvexShape m_glassBase;     // corps principal semi-transparent
    sf::ConvexShape m_glassHighlight;// bande de reflet en haut (effet verre)
    sf::ConvexShape m_borderOutline; // contour fin lumineux (dessine en "wireframe")

    sf::Text m_text;

    // --- Couleurs (glassmorphism : transparence + teinte froide) ---
    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_borderColorNormal;
    sf::Color m_borderColorHover;
    sf::Color m_highlightColor;

    // --- Geometrie ---
    sf::Vector2f m_size;
    float m_cornerRadius;

    // --- Animation ---
    float m_currentScale;
    float m_targetScale;
    float m_currentGlow;   // intensite de la bordure (0 = normal, 1 = survol)
    float m_targetGlow;
};