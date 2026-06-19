#include "Controller.h"
#include "MenuScreen.h"
#include "Resources.h"   
#include "GameplayScreen.h"
#include "GameException.h"
#include "AudioManager.h"
#include "AssetLoader.h"
#include <optional>

Controller::Controller()
    : m_window(sf::VideoMode({ 1200, 600 }), "Jetpack Joyride") {

    AssetLoader::loadAll();

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