#include "MenuScreen.h"
#include "GameplayScreen.h"
#include "Resources.h"
#include "RulesScreen.h"

MenuScreen::MenuScreen(ScreenStack& stack)
    : Screen(stack),
    m_backgroundSprite(Resources::getInstance().getTexture("start_background")),
    m_titleSprite(Resources::getInstance().getTexture("Title")), // Initialisation obligatoire du titre
    m_mousePos(0.f, 0.f)
{
    auto& resources = Resources::getInstance();

    // 1. Redimensionnement parfait du fond d'écran (1200x600)
    auto bgSize = resources.getTexture("start_background").getSize();
    m_backgroundSprite.setScale({ 1200.f / bgSize.x, 600.f / bgSize.y });

    // 2. CONFIGURATION DU LOGO (Placé sur la gauche)
    auto titleSize = resources.getTexture("Title").getSize();
    m_titleSprite.setOrigin({ static_cast<float>(titleSize.x) / 2.f, static_cast<float>(titleSize.y) / 2.f });
    m_titleSprite.setPosition({ 350.f, 300.f }); // Centré à X=350 (gauche) et Y=300 (milieu vertical)

    // 3. CONFIGURATION DES BOUTONS (Décalés sur la droite à X = 900.f)
    const auto& font = resources.getFont();
    sf::Vector2f btnSize{ 260.0f, 60.0f };

    m_playButton = std::make_unique<Button>(sf::Vector2f{ 900.f, 250.f }, btnSize, "Play", font);
    m_rulesButton = std::make_unique<Button>(sf::Vector2f{ 900.f, 350.f }, btnSize, "Rules", font);
    m_highScoreButton = std::make_unique<Button>(sf::Vector2f{ 900.f, 445.f }, btnSize, "High Score", font);
}

void MenuScreen::handleEvent(const sf::Event& event) {
    // Capture de la position de la souris (Norme SFML 3.0)
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) };
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x), static_cast<float>(mouseReleased->position.y) };

            if (m_playButton->isClicked(clickPos)) {
                m_stack.push(std::make_unique<GameplayScreen>(m_stack));
            }
            else if (m_rulesButton->isClicked(clickPos)) {
                m_stack.push(std::make_unique<RulesScreen>(m_stack));
            }
        }
    }
}

void MenuScreen::update(float deltaTime) {
    m_playButton->update(m_mousePos, deltaTime);
    m_rulesButton->update(m_mousePos, deltaTime);
    m_highScoreButton->update(m_mousePos, deltaTime);
}

void MenuScreen::draw(sf::RenderWindow& window) {
    // Rendu des couches dans l'ordre de superposition
    window.draw(m_backgroundSprite);
    window.draw(m_titleSprite); // Dessin du logo sur la partie gauche

    m_playButton->draw(window);
    m_rulesButton->draw(window);
    m_highScoreButton->draw(window);
}