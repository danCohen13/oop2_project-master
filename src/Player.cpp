#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"
#include "DeadState.h"
#include "BoostState.h" 
#include "AudioManager.h"

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isDead(false),
    m_isThrusting(false),
    m_isInvincible(false), // Par défaut, le joueur peut mourir
    m_state(std::make_unique<WalkState>()),
    m_currentMovement(MovementType::Walking),
    m_floorY(550.0f)
{
    m_sprite.setPosition({ 100.0f, 300.0f });

    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = static_cast<int>(playerTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(playerTex.getSize().y);
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { playerWidth, playerHeight }));

    m_floorY = 600.0f - static_cast<float>(playerHeight) - 20.0f;
}

void Player::update(float deltaTime, bool isThrusting) {
    if (m_isDead && m_currentMovement != MovementType::Dying && m_currentMovement != MovementType::Boosting) {
        m_state = std::make_unique<DeadState>();
        m_currentMovement = MovementType::Dying;
        m_isThrusting = false;
        AudioManager::getInstance().playSound("hit");
    }

    // 1. CALCULS PHYSIQUES
    if (m_currentMovement == MovementType::Dying) {
        m_verticalVelocity += GRAVITY * deltaTime;
    }
    else {
        m_isThrusting = isThrusting;
        m_verticalVelocity += GRAVITY * deltaTime;
        if (m_isThrusting) {
            m_verticalVelocity = JETPACK_FORCE;
        }
    }

    m_sprite.move({ 0.0f, m_verticalVelocity * deltaTime });

    sf::Vector2f position = m_sprite.getPosition();
    if (position.y >= m_floorY) {
        m_sprite.setPosition({ position.x, m_floorY });
        m_verticalVelocity = 0.0f;
    }
    else if (position.y <= CEILING_Y) {
        m_sprite.setPosition({ position.x, CEILING_Y });
        if (m_currentMovement != MovementType::Dying) m_verticalVelocity = 0.0f;
    }

    // 2. GESTION DES TRANSITIONS DE MOUVEMENT STANDARDS
    if (m_currentMovement != MovementType::Dying && m_currentMovement != MovementType::Boosting) {
        bool isOnGround = (m_sprite.getPosition().y >= m_floorY);

        if (isOnGround && m_currentMovement != MovementType::Walking) {
            m_state = std::make_unique<WalkState>();
            m_currentMovement = MovementType::Walking;
        }
        else if (!isOnGround && m_currentMovement != MovementType::Jumping) {
            m_state = std::make_unique<JumpState>();
            m_currentMovement = MovementType::Jumping;
        }
    }

    m_state->update(*this, deltaTime);
}

void Player::draw(sf::RenderWindow& window) const {
    m_state->draw(window, m_sprite, m_exhaust);
}

void Player::activateSpeedBoost(float distanceInPixels) {
    if (m_currentMovement == MovementType::Dying) return;

    m_currentMovement = MovementType::Boosting;
    m_isInvincible = true; // Rendu immédiatement invincible
    m_state = std::make_unique<BoostState>(m_sprite.getPosition().x, distanceInPixels);

    AudioManager::getInstance().playSound("shield_speed");
}

void Player::stopBoost() {
    // À la fin de l'état graphique du boost, l'invincibilité reste à true.
    // C'est maintenant la GameSession qui décidera quand la couper !
    bool isOnGround = (m_sprite.getPosition().y >= m_floorY);
    if (isOnGround) {
        m_state = std::make_unique<WalkState>();
        m_currentMovement = MovementType::Walking;
    }
    else {
        m_state = std::make_unique<JumpState>();
        m_currentMovement = MovementType::Jumping;
    }
}

void Player::setDead(bool dead) {
    if (m_isInvincible) return; // Protection absolue active
    m_isDead = dead;
}

void Player::collide(Object& other) { other.collide(*this); }
bool Player::isDead() const { return m_isDead; }
bool Player::isThrusting() const { return m_isThrusting; }