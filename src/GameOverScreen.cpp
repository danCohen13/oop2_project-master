#include "GameOverScreen.h"
#include "GameplayScreen.h"
#include "Resources.h"

GameOverScreen::GameOverScreen(ScreenStack& stack, int coins, int distance)
    : Screen(stack),
    m_gameOverText(Resources::getInstance().getFont()),
    m_statsText(Resources::getInstance().getFont()),
    m_mousePos(0.f, 0.f),
    m_coins(coins),
    m_distance(distance)
{
    // 1. Fond d'atténuation assombri sur l'ensemble de la vue (1200x600)
    m_overlay.setSize({ 1200.f, 600.f });
    m_overlay.setFillColor(sf::Color(15, 15, 25, 215));

    // 2. Configuration du titre "GAME OVER"
    m_gameOverText.setString("YOU LOST !");
    m_gameOverText.setCharacterSize(55);
    m_gameOverText.setFillColor(sf::Color(255, 45, 45));
    auto goSize = m_gameOverText.getLocalBounds().size;
    m_gameOverText.setOrigin({ goSize.x / 2.f, goSize.y / 2.f });
    m_gameOverText.setPosition({ 600.f, 130.f });

    // 3. Configuration de l'affichage des scores et statistiques
    std::string statsStr = "Coins Collected : " + std::to_string(m_coins) + "\n" +
        "Distance Traveled : " + std::to_string(m_distance) + " m\n\n" +
        "Best Score : " + std::to_string(m_coins) + " (Placeholder)";
    m_statsText.setString(statsStr);
    m_statsText.setCharacterSize(24);
    m_statsText.setFillColor(sf::Color::White);
    auto statsSize = m_statsText.getLocalBounds().size;
    m_statsText.setOrigin({ statsSize.x / 2.f, statsSize.y / 2.f });
    m_statsText.setPosition({ 600.f, 270.f });

    // 4. Instanciation des boutons bleus interactifs
    const auto& font = Resources::getInstance().getFont();
    sf::Vector2f btnSize{ 260.0f, 60.0f };
    m_restartButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 410.f }, btnSize, "Restart", font);
    m_menuButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 495.f }, btnSize, "Menu", font);
}

void GameOverScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) };
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x), static_cast<float>(mouseReleased->position.y) };

            // CORRECTION SÉCURITÉ RESTART
            if (m_restartButton->isClicked(clickPos)) {
                auto& safeStack = m_stack; // 1. Sauvegarde de la référence locale (vit sur le registre processeur)
                safeStack.pop();           // 2. Supprime l'écran GameOverScreen (Détruit l'objet courant, 'this' n'existe plus !)
                safeStack.pop();           // 3. Supprime l'ancien GameplayScreen de manière sécurisée
                safeStack.push(std::make_unique<GameplayScreen>(safeStack)); // 4. Pousse le nouveau niveau
                return;                    // 5. Quitte immédiatement pour ne plus toucher à la mémoire détruite
            }

            // CORRECTION SÉCURITÉ MENU (L'erreur venait d'ici !)
            else if (m_menuButton->isClicked(clickPos)) {
                auto& safeStack = m_stack; // 1. Sauvegarde de la référence locale
                safeStack.pop();           // 2. Supprime l'écran GameOverScreen
                safeStack.pop();           // 3. Supprime le GameplayScreen -> Découvre le MenuScreen qui était en dessous
                return;                    // 4. Quitte immédiatement de manière propre !
            }
        }
    }
}

void GameOverScreen::update(float deltaTime) {
    m_restartButton->update(m_mousePos, deltaTime);
    m_menuButton->update(m_mousePos, deltaTime);
}

void GameOverScreen::draw(sf::RenderWindow& window) {
    window.draw(m_overlay);
    window.draw(m_gameOverText);
    window.draw(m_statsText);
    m_restartButton->draw(window);
    m_menuButton->draw(window);
}