#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textStr, const sf::Font& font);

    void update(const sf::Vector2f& mousePos, float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2f& mousePos) const;

    void setPosition(const sf::Vector2f& position);
    sf::FloatRect getGlobalBounds() const;

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;

    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_outlineColor;

    float m_currentScale;
    float m_targetScale;
};