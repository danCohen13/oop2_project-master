#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"
#include "DeadState.h"
#include "AudioManager.h"

// Initialisation de la variable globale partagée
bool Player::s_isSpeedBoosting = false;

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isDead(false),
    m_isThrusting(false),
    m_state(std::make_unique<WalkState>()),
    m_currentMovement(MovementType::Walking),
    m_floorY(550.0f),
    m_isSpeedBoosting(false),
    m_speedBoostTargetX(0.0f),
    m_speedFlameAnimator(Resources::getInstance().getTexture("speedflame"), 4, 0.05f) // Découpe en 4 frames
{
    m_sprite.setPosition({ 100.0f, 300.0f });

    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = static_cast<int>(playerTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(playerTex.getSize().y);
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { playerWidth, playerHeight }));

    m_floorY = 600.0f - static_cast<float>(playerHeight) - 20.0f;
}

void Player::update(float deltaTime, bool isThrusting) {
    // Vérification de la distance restante si le mode boost est actif
    if (m_isSpeedBoosting) {
        if (m_sprite.getPosition().x >= m_speedBoostTargetX) {
            m_isSpeedBoosting = false;
            s_isSpeedBoosting = false;
            m_sprite.setRotation(sf::degrees(0.f)); // Remise droite (SFML 3.0)
        }
        else {
            m_sprite.setRotation(sf::degrees(15.f)); // Inclinaison aérodynamique
            m_speedFlameAnimator.update(deltaTime);
        }
    }

    if (m_isDead && m_currentMovement != MovementType::Dying) {
        m_state = std::make_unique<DeadState>();
        m_currentMovement = MovementType::Dying;
        m_isThrusting = false;
        AudioManager::getInstance().playSound("hit");
    }

    // 1. CALCULS PHYSIQUES (Le joueur continue de monter/descendre normalement)
    if (m_isDead) {
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
        if (!m_isDead) m_verticalVelocity = 0.0f;
    }

    // 2. GESTION DES TRANSITIONS ANIMÉES (Uniquement si vivant)
    if (!m_isDead) {
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
    if (m_isSpeedBoosting) {
        // Dessin unique sans afficher l'exhaust d'origine
        window.draw(m_sprite);

        // Configuration et dessin de la super-flamme bleue derrière son dos
        sf::Sprite flameSprite(Resources::getInstance().getTexture("speedflame"));
        m_speedFlameAnimator.applyTo(flameSprite);
        flameSprite.setPosition(m_sprite.getPosition() + sf::Vector2f(-35.f, 15.f));
        flameSprite.setRotation(m_sprite.getRotation());
        window.draw(flameSprite);
    }
    else {
        m_state->draw(window, m_sprite, m_exhaust);
    }
}

void Player::activateSpeedBoost(float distanceInPixels) {
    if (m_isDead) return;
    m_isSpeedBoosting = true;
    s_isSpeedBoosting = true;
    m_speedBoostTargetX = m_sprite.getPosition().x + distanceInPixels;

    // Déclenche le son de bouclier/vitesse déjà présent dans tes ressources
    AudioManager::getInstance().playSound("shield_speed");
}

void Player::setDead(bool dead) {
    if (m_isSpeedBoosting) return; // IGNORER LA MORT : Totalement invincible !
    m_isDead = dead;
}

void Player::collide(Object& other) { other.collide(*this); }
bool Player::isDead() const { return m_isDead; }
bool Player::isThrusting() const { return m_isThrusting; }