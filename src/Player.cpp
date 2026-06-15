#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isDead(false),
    m_isThrusting(false),
    // CORRECTION : m_flameSprite a été retiré d'ici car m_exhaust s'initialise tout seul !
    m_floorY(550.0f),
    m_state(std::make_unique<WalkState>()),
    m_currentMovement(MovementType::Walking)
{
    m_sprite.setPosition({ 100.0f, 300.0f });

    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = playerTex.getSize().x / 4;
    int playerHeight = playerTex.getSize().y;
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { playerWidth, playerHeight }));

    m_floorY = 600.0f - static_cast<float>(playerHeight) - 20.0f;

    // CORRECTION : Tout l'ancien code de découpage manuel de m_flameSprite a été supprimé.
}

void Player::update(float deltaTime, bool isThrusting) {
    if (m_isDead) return;

    m_isThrusting = isThrusting;

    // 1. CALCULS PHYSIQUES
    m_verticalVelocity += GRAVITY * deltaTime;

    if (m_isThrusting) {
        m_verticalVelocity = JETPACK_FORCE;
    }

    m_sprite.move({ 0.0f, m_verticalVelocity * deltaTime });

    // Gestion des collisions avec les bordures
    sf::Vector2f position = m_sprite.getPosition();
    if (position.y >= m_floorY) {
        m_sprite.setPosition({ position.x, m_floorY });
        m_verticalVelocity = 0.0f;
    }
    else if (position.y <= CEILING_Y) {
        m_sprite.setPosition({ position.x, CEILING_Y });
        m_verticalVelocity = 0.0f;
    }

    // 2. GESTION DES TRANSITIONS
    bool isOnGround = (m_sprite.getPosition().y >= m_floorY);

    if (isOnGround && m_currentMovement != MovementType::Walking) {
        m_state = std::make_unique<WalkState>();
        m_currentMovement = MovementType::Walking;
    }
    else if (!isOnGround && m_currentMovement != MovementType::Jumping) {
        m_state = std::make_unique<JumpState>();
        m_currentMovement = MovementType::Jumping;
    }

    // 3. DELEGATION DE L'ANIMATION À L'ÉTAT
    m_state->update(*this, deltaTime);
}

void Player::draw(sf::RenderWindow& window) const {
    // CORRECTION : On passe l'objet exhaust à la fonction de dessin de l'état
    m_state->draw(window, m_sprite, m_exhaust);
}

void Player::collide(Object& other) {
    other.collide(*this);
}

bool Player::isDead() const { return m_isDead; }
void Player::setDead(bool dead) { m_isDead = dead; }
bool Player::isThrusting() const { return m_isThrusting; }