#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"
#include "BoostState.h"
#include "SuperPowerRunnerState.h"
#include "PlayerState.h"
#include "AudioManager.h"

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isThrusting(false),
    m_isInvincible(false),
    m_state(std::make_unique<WalkState>()),
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
    m_isThrusting = isThrusting;

    // 1. Mise à jour de l'état actif
    m_state->update(*this, deltaTime);

    // 2. RÈGLE DU DÉPÔT : Mise à jour physique autonome de la position de la flamme
    m_exhaust.update(deltaTime, m_sprite.getPosition());
}

void Player::draw(sf::RenderWindow& window) const {
    m_state->draw(window, m_sprite, m_exhaust);
}

void Player::changeState(std::unique_ptr<PlayerState> newState) {
    if (newState) {
        m_state = std::move(newState);
    }
}

void Player::applyStandardPhysics(float deltaTime, bool useJetpack) {
    m_verticalVelocity += GRAVITY * deltaTime;
    if (useJetpack && m_isThrusting) {
        m_verticalVelocity = JETPACK_FORCE;
    }

    m_sprite.move({ 0.0f, m_verticalVelocity * deltaTime });

    sf::Vector2f position = m_sprite.getPosition();
    if (position.y >= m_floorY) {
        m_sprite.setPosition({ position.x, m_floorY });
        m_verticalVelocity = 0.0f;
    }
    else if (position.y <= CEILING_Y) {
        m_sprite.setPosition({ position.x, CEILING_Y });
        m_verticalVelocity = 0.0f;
    }
}

void Player::activateSpeedBoost(float distanceInPixels) {
    if (isDead() || isSuperPowerRunner()) {
        return; 
    }
    
    changeState(std::make_unique<BoostState>(m_sprite.getPosition().x, distanceInPixels));
    AudioManager::getInstance().playSound("shield_speed");
}

void Player::stopBoost() {
    if (m_sprite.getPosition().y >= m_floorY) {
        changeState(std::make_unique<WalkState>());
    }
    else {
        changeState(std::make_unique<JumpState>());
    }
}

void Player::activateSuperPowerRunner() {
    // LE VERROU INVERSE : Si le joueur est en plein boost, il ne peut pas mettre le costume.
    if (isDead() || isSpeedBoosting()) {
        return;
    }

    changeState(std::make_unique<SuperPowerRunnerState>(*this));
}

void Player::setDead(bool dead) {
    if (dead) {
        m_state->handleHit(*this);
    }
}

bool Player::isDead()            const { return m_state->isDead(); }
bool Player::isThrusting()       const { return m_isThrusting; }
bool Player::isSpeedBoosting()   const { return m_state->isSpeedBoosting(); }
bool Player::isSuperPowerRunner() const { return m_state->isSuperPowerRunner(); }
bool Player::isInvincible()      const { return m_isInvincible || m_state->isInvincible(); }

void Player::collide(Object& other) { other.collide(*this); }