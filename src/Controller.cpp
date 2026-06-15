#include "Controller.h"
#include "MenuScreen.h"
#include "Resources.h"   
#include "GameplayScreen.h"
#include <optional>

Controller::Controller()
    : m_window(sf::VideoMode({ 1200, 600 }), "Jetpack Joyride") {

    // 1. CHARGEMENT DES ASSETS DEPUIS LE DOSSIER RESOURCES
    auto& resources = Resources::getInstance();
    resources.loadTexture("warehouse_background", "resources/warehouse_background.png");
    resources.loadTexture("start_background", "resources/GameMenu.png");
    resources.loadTexture("background", "resources/Hall.png");
    resources.loadTexture("player", "resources/JetpackWalking.png");
    resources.loadTexture("PlayerDead", "resources/PlayerDead.png"); 
    resources.loadFont("resources/New Athletic M54.ttf");
    resources.loadTexture("Exhaust", "resources/Exhaust.png");
    resources.loadTexture("Coin", "resources/Coin.png");
    resources.loadTexture("Laser", "resources/Laser.png");
    resources.loadTexture("Missile", "resources/Missile.png");
    resources.loadTexture("MissileWarning", "resources/MissileWarning.png");
    resources.loadTexture("MissileIncoming", "resources/MissileIncoming.png");


    // 2. ENCHAINEMENT SUR LE PREMIER ÉCRAN
    m_screens.push(std::make_unique<MenuScreen>(m_screens));
}

void Controller::run() {
    m_clock.restart();

    while (m_window.isOpen() && !m_screens.empty()) {
        float deltaTime = m_clock.restart().asSeconds();

        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }
            m_screens.top()->handleEvent(*event);
        }

        if (m_screens.empty()) break;

        m_screens.top()->update(deltaTime);

        if (m_screens.empty()) break;

        m_window.clear(sf::Color::Black);
        m_screens.top()->draw(m_window);
        m_window.display();
    }
}