#include "Controller.h"
#include "MenuScreen.h"
#include "Resources.h"   
#include "GameplayScreen.h"
#include "GameException.h"
#include "AudioManager.h"
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

    resources.loadTexture("play_btn", "resources/play.png");
    resources.loadTexture("rules_btn", "resources/rules.png");
    resources.loadTexture("button_menu", "resources/ButtonMenu.png");
    resources.loadTexture("Title", "resources/Title.png");
    resources.loadTexture("help1", "resources/help1.PNG");
    resources.loadTexture("help2", "resources/help2.PNG");
    resources.loadTexture("help3", "resources/help3.PNG");
    resources.loadTexture("back_btn", "resources/back.png");
    resources.loadTexture("next_btn", "resources/continue.png");
    resources.loadTexture("Scientist", "resources/Scientist.png");

    resources.loadSound("missile_alarm", "resources/MissileAlarm.wav");
    resources.loadSound("piggy", "resources/Piggy.wav");
    resources.loadSound("shield_speed", "resources/ShieldSpeed.ogg");
    resources.loadSound("zapper", "resources/ZapperSound.wav");
    resources.loadSound("coin", "resources/coin.wav");
    resources.loadSound("get_power", "resources/getPowerSound.wav");
    resources.loadSound("hit", "resources/hitSound.wav");
    resources.loadSound("missile_launch", "resources/missileLaunch.wav");
    resources.loadTexture("Speed", "resources/SpeedGift.png");
    resources.loadTexture("speedflame", "resources/speedflame.png");

    Resources::getInstance().loadTexture("PowerUpBox", "resources/PowerUpBox.png");
    Resources::getInstance().loadTexture("SuperPowerRunner", "resources/SuperPowerRunner.png");

    resources.loadTexture("pause_btn", "resources/pause.png");

    AudioManager::getInstance().playMusic("resources/JetpackJoyrideMusic.wav", 35.f);
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