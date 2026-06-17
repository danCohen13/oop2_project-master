#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"
#include "DeadState.h"
#include "BoostState.h" 
#include "SuperPowerRunnerState.h" // <-- CORRECTION 1 : Inclusion requise pour l'allocation d'état
#include "AudioManager.h"

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isDead(false),
    m_isThrusting(false),
    m_isInvincible(false),
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

        // CORRECTION GÉOMÉTRIQUE CRUCIALE : On remet les propriétés du sprite à zéro 
        // pour que DeadState puisse l'animer et le plaquer au sol sans conflit d'origine
        m_sprite.setOrigin({ 0.f, 0.f });
        m_sprite.setScale({ 1.f, 1.f });
        m_sprite.setRotation(sf::degrees(0.f)); // SFML 3.0 kompatibel

        AudioManager::getInstance().playSound("hit");
    }

    // FIX INTÉGRATION : Enregistré au début pour être lu par le SuperPowerRunnerState
    m_isThrusting = isThrusting;

    // 1. CALCULS PHYSIQUES STANDARDS (Bypassés si le joueur est en mode SuperPowerRunner)
    if (m_currentMovement != MovementType::SuperPowerRunner) {
        if (m_currentMovement == MovementType::Dying) {
            m_verticalVelocity += GRAVITY * deltaTime;
        }
        else {
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
    }

    // 2. GESTION DES TRANSITIONS DE MOUVEMENT STANDARDS (Protégé contre le reset automatique)
    if (m_currentMovement != MovementType::Dying &&
        m_currentMovement != MovementType::Boosting &&
        m_currentMovement != MovementType::SuperPowerRunner) {

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
    m_isInvincible = true;
    m_state = std::make_unique<BoostState>(m_sprite.getPosition().x, distanceInPixels);

    AudioManager::getInstance().playSound("shield_speed");
}

void Player::stopBoost() {
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

void Player::activateSuperPowerRunner() {
    if (m_isDead) return;
    m_currentMovement = MovementType::SuperPowerRunner;
    m_state = std::make_unique<SuperPowerRunnerState>(*this);
}

void Player::setDead(bool dead) {
    if (m_isInvincible) return;

    // PROTECTION : Si le joueur se fait toucher pendant qu'il a le costume de gravité
    if (m_currentMovement == MovementType::SuperPowerRunner) {
        const sf::Texture& normalTex = Resources::getInstance().getTexture("player");

        // 1. Réajustement de la hauteur : On compense le fait que le runner avait
        // son origine fixée aux pieds alors que le joueur normal l'a en haut à gauche (0,0)
        sf::Vector2f pos = m_sprite.getPosition();
        pos.y -= static_cast<float>(normalTex.getSize().y);
        m_sprite.setPosition(pos);

        // 2. Réinitialisation complète du sprite pour le joueur normal
        m_sprite.setTexture(normalTex, false);
        m_sprite.setOrigin({ 0.f, 0.f });       // Remet l'origine en haut à gauche (l'exhaust se recale bien !)
        m_sprite.setScale({ 1.f, 1.f });        // Remet la tête en haut
        m_sprite.setRotation(sf::degrees(0.f)); // Remise droite (SFML 3.0)

        m_verticalVelocity = 0.f; // On amortit sa vitesse de chute initiale

        // 3. Redirection automatique vers l'état adéquat (au sol ou dans les airs)
        bool isOnGround = (m_sprite.getPosition().y >= m_floorY);
        if (isOnGround) {
            m_state = std::make_unique<WalkState>();
            m_currentMovement = MovementType::Walking;
        }
        else {
            m_state = std::make_unique<JumpState>();
            m_currentMovement = MovementType::Jumping;
        }
        return; // On quitte la fonction SANS appliquer m_isDead = true !
    }

    m_isDead = dead;
}

void Player::collide(Object& other) { other.collide(*this); }
bool Player::isDead() const { return m_isDead; }
bool Player::isThrusting() const { return m_isThrusting; }