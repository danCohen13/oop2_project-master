#include "GameSession.h"
#include <SFML/Audio.hpp>
#include "AudioManager.h"
#include "Resources.h"

GameSession::GameSession()
    : m_score(0), m_lives(1), m_gameSpeed(140.0f)
{
}

void GameSession::update(float deltaTime, bool isThrusting) {
    // 1. GESTION DE LA VITESSE ET DU FREINAGE (FRICTION)
    if (!isGameOver()) {
        if (Player::isAnyPlayerBoosting()) {
            m_gameSpeed = 1600.0f; // Vitesse fulgurante Hyper-Drive !
        }
        else if (m_gameSpeed < 900.0f) {
            m_gameSpeed += 3.5f * deltaTime;
        }
        else if (m_gameSpeed > 900.0f) {
            m_gameSpeed -= 400.0f * deltaTime; // Ralentissement fluide après la fin du boost
            if (m_gameSpeed < 900.0f) m_gameSpeed = 900.0f;
        }
    
    else {
        // Si mort, on freine progressivement le défilement pour un arrêt dramatique
        if (m_gameSpeed > 0.0f) {
            m_gameSpeed -= 120.0f * deltaTime;
            if (m_gameSpeed < 0.0f) {
                m_gameSpeed = 0.0f;
            }
        }
    }

    // 2. Mise à jour physique du monde
    m_board.play(deltaTime, m_gameSpeed, isThrusting);

    // 3. RÈGLES DE JEU (Uniquement si la partie est encore active !)
    if (!isGameOver()) {
        int coins = m_board.getCoinsCollectedThisFrame();
        if (coins > 0) {
            addScore(1);

            AudioManager::getInstance().playSound("coin");
        }

        // CORRECTION : N'est lu que si le jeu n'est pas déjà considéré en Game Over
        if (m_board.hasPlayerHitHazard()) {
            removeLife(); // Appelé une seule fois, m_lives passe à 0, bloquant le prochain tick !
        }
    }
}

void GameSession::drawWorld(sf::RenderWindow& window) const {
    m_board.draw(window);
}

int GameSession::getScore() const { return m_score; }
int GameSession::getLives() const { return m_lives; }
bool GameSession::isGameOver() const { return m_lives <= 0 || !m_board.isPlayerAlive(); }
sf::Vector2f GameSession::getPlayerPosition() const { return m_board.getPlayerPosition(); }

int GameSession::getDistanceInMeters() const {
    float playerX = getPlayerPosition().x;
    float distanceInPixels = playerX - 100.0f;
    int meters = static_cast<int>(distanceInPixels / 100.0f);
    return (meters < 0) ? 0 : meters;
}

void GameSession::addScore(int points) { m_score += points; }
void GameSession::removeLife() { m_lives--; }