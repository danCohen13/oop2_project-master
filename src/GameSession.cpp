#include "GameSession.h"
#include <SFML/Audio.hpp>
#include "AudioManager.h"
#include "Resources.h"
#include "Player.h"

GameSession::GameSession()
    : m_score(0), m_lives(1), m_gameSpeed(140.0f), m_normalGameSpeed(140.0f), m_boostCooldownTimer(0.0f)
{
}

void GameSession::update(float deltaTime, bool isThrusting) {
    Player* player = m_board.getPlayer(); // On récupère l'instance du joueur pour ajuster son invincibilité

    // 1. GESTION DE LA VITESSE ET DU FREINAGE (FRICTION)
    if (!isGameOver()) {
        if (m_normalGameSpeed < 900.0f) {
            m_normalGameSpeed += 3.5f * deltaTime;
        }

        if (player && player->isSpeedBoosting()) {
            m_gameSpeed = 1600.0f;
            m_boostCooldownTimer = 2.5f; // On arme le timer à 2.5 secondes de sécurité
            player->setInvincible(true);  // Sécurité redondante
        }
        else {
            if (m_gameSpeed > m_normalGameSpeed) {
                m_gameSpeed -= 600.0f * deltaTime; // Décélération progressive
                if (m_gameSpeed < m_normalGameSpeed) {
                    m_gameSpeed = m_normalGameSpeed;
                }
                if (player) player->setInvincible(true); // Toujours invincible pendant l'entre-deux !
            }
            else {
                m_gameSpeed = m_normalGameSpeed;

                // On est enfin revenu à la vitesse normale ! Le cooldown de 2.5s commence à expirer
                if (m_boostCooldownTimer > 0.0f) {
                    m_boostCooldownTimer -= deltaTime;
                    if (player) player->setInvincible(true); // Reste protégé pendant les 2-3 secondes
                }
                else {
                    // Le délai est totalement écoulé, le joueur redevient mortel de manière sécurisée
                    if (player && player->isInvincible()) {
                        player->setInvincible(false);
                    }
                }
            }
        }
    }
    else {
        if (m_gameSpeed > 0.0f) {
            m_gameSpeed -= 120.0f * deltaTime;
            if (m_gameSpeed < 0.0f) m_gameSpeed = 0.0f;
        }
    }

    // 2. Mise à jour physique du monde
    m_board.play(deltaTime, m_gameSpeed, isThrusting);

    // 3. RÈGLES DE JEU
    if (!isGameOver()) {
        int coins = m_board.getCoinsCollectedThisFrame();
        if (coins > 0) {
            addScore(1);
            AudioManager::getInstance().playSound("coin");
        }

        if (m_board.hasPlayerHitHazard()) {
            removeLife();
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