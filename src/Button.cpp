#include "Button.h"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textStr, const sf::Font& font)
    : m_text(font),
    m_normalColor(sf::Color(12, 45, 96)),       // Bleu nuit technologique
    m_hoverColor(sf::Color(24, 84, 170)),       // Bleu électrique lumineux au survol
    m_outlineColor(sf::Color(0, 210, 255)),     // Bordure néon Cyan
    m_currentScale(1.0f),
    m_targetScale(1.0f)
{
    // Configuration de la boîte du bouton
    m_shape.setSize(size);
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineColor(m_outlineColor);
    m_shape.setOutlineThickness(3.0f);
    m_shape.setOrigin({ size.x / 2.0f, size.y / 2.0f });
    m_shape.setPosition(position);

    // Configuration du texte
    m_text.setString(textStr);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);

    // Centrage absolu du texte dans le bouton
    auto textSize = m_text.getLocalBounds().size; // Norme SFML 3.0
    m_text.setOrigin({ textSize.x / 2.0f, textSize.y / 2.0f });
    m_text.setPosition(position);
}

void Button::update(const sf::Vector2f& mousePos, float deltaTime) {
    // Détection du survol de la souris
    if (m_shape.getGlobalBounds().contains(mousePos)) {
        m_shape.setFillColor(m_hoverColor);
        m_targetScale = 1.25f; // Les lettres s'agrandissent de 25%
    }
    else {
        m_shape.setFillColor(m_normalColor);
        m_targetScale = 1.0f;  // Retour à la taille normale
    }

    // Animation fluide (Lerp) pour éviter les changements brusques
    m_currentScale += (m_targetScale - m_currentScale) * 12.0f * deltaTime;
    m_text.setScale({ m_currentScale, m_currentScale });
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
    window.draw(m_text);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return m_shape.getGlobalBounds().contains(mousePos);
}

void Button::setPosition(const sf::Vector2f& position) {
    m_shape.setPosition(position);
    m_text.setPosition(position);
}

sf::FloatRect Button::getGlobalBounds() const {
    return m_shape.getGlobalBounds();
}