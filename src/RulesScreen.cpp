#include "RulesScreen.h"
#include "Resources.h"

RulesScreen::RulesScreen(ScreenStack& stack)
    : Screen(stack),
    m_backgroundSprite(Resources::getInstance().getTexture("help1")),
    m_currentPage(0),
    m_pages({ "help1", "help2", "help3" }),
    m_mousePos(0.f, 0.f)
{
    // Redimensionnement de l'image d'aide initiale
    auto bgSize = Resources::getInstance().getTexture("help1").getSize();
    m_backgroundSprite.setScale({ 1200.f / bgSize.x, 600.f / bgSize.y });

    const auto& font = Resources::getInstance().getFont();

    // Boutons stylisés bleus interactifs
    m_backButton = std::make_unique<Button>(sf::Vector2f{ 120.0f, 50.0f }, sf::Vector2f{ 160.0f, 50.0f }, "Back", font);

    // CORRECTION EMPLACEMENT : Le bouton continue est replacé idéalement à l'intérieur de la vue
    m_nextButton = std::make_unique<Button>(sf::Vector2f{ 1050.0f, 530.0f }, sf::Vector2f{ 200.0f, 50.0f }, "Continue", font);
}

void RulesScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) };
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x), static_cast<float>(mouseReleased->position.y) };

            if (m_backButton->isClicked(clickPos)) {
                m_stack.pop();
            }
            else if (m_currentPage < 2 && m_nextButton->isClicked(clickPos)) {
                m_currentPage++;

                const auto& nextTexture = Resources::getInstance().getTexture(m_pages[m_currentPage]);
                m_backgroundSprite.setTexture(nextTexture);

                auto bgSize = nextTexture.getSize();
                m_backgroundSprite.setScale({ 1200.f / bgSize.x, 600.f / bgSize.y });
            }
        }
    }
}

void RulesScreen::update(float deltaTime) {
    m_backButton->update(m_mousePos, deltaTime);
    if (m_currentPage < 2) {
        m_nextButton->update(m_mousePos, deltaTime);
    }
}

void RulesScreen::draw(sf::RenderWindow& window) {
    window.draw(m_backgroundSprite);
    m_backButton->draw(window);

    if (m_currentPage < 2) {
        m_nextButton->draw(window);
    }
}