#pragma once
#include <SFML/Graphics.hpp>
#include <string>

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
    static sf::ConvexShape makeRoundedRect(const sf::Vector2f& size, float radius);

    sf::ConvexShape m_glassBase;     
    sf::ConvexShape m_glassHighlight;
    sf::ConvexShape m_borderOutline; 

    sf::Text m_text;

    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_borderColorNormal;
    sf::Color m_borderColorHover;
    sf::Color m_highlightColor;

    sf::Vector2f m_size;
    float m_cornerRadius;

    float m_currentScale;
    float m_targetScale;
    float m_currentGlow;   
    float m_targetGlow;
};