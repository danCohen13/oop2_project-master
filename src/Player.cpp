#include "Player.h"
#include "Resources.h"
#include "WalkState.h"
#include "JumpState.h"
#include "DeadState.h" // CORRECTION : Ne pas oublier d'inclure le DeadState !

Player::Player()
    : MovingGameObject(Resources::getInstance().getTexture("player"), 300.0f),
    m_verticalVelocity(0.0f),
    m_isDead(false),
    m_isThrusting(false),
    m_floorY(550.0f),
    m_state(std::make_unique<WalkState>()),
    m_currentMovement(MovementType::Walking)
{
    m_sprite.setPosition({ 100.0f, 300.0f });

    const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
    int playerWidth = static_cast<int>(playerTex.getSize().x) / 4;
    int playerHeight = static_cast<int>(playerTex.getSize().y);
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 }, { playerWidth, playerHeight }));

    m_floorY = 600.0f - static_cast<float>(playerHeight) - 20.0f;
}

void Player::update(float deltaTime, bool isThrusting) {
    // CORRECTION : Plus de return hâtif. On gère la transition vers l'état de mort
    if (m_isDead && m_currentMovement != MovementType::Dying) {
        m_state = std::make_unique<DeadState>();
        m_currentMovement = MovementType::Dying;
        m_isThrusting = false;
    }

    // 1. CALCULS PHYSIQUES
    if (m_isDead) {
        // Si le joueur est mort, il n'a plus de jetpack, il subit juste la gravité
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

    // Gestion des collisions avec les bordures
    sf::Vector2f position = m_sprite.getPosition();
    if (position.y >= m_floorY) {
        m_sprite.setPosition({ position.x, m_floorY });
        m_verticalVelocity = 0.0f;
    }
    else if (position.y <= CEILING_Y) {
        m_sprite.setPosition({ position.x, CEILING_Y });
        if (!m_isDead) m_verticalVelocity = 0.0f;
    }

    // 2. GESTION DES TRANSITIONS (Uniquement si vivant)
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

    // 3. DELEGATION DE L'ANIMATION À L'ÉTAT (Fonctionne aussi pour le DeadState)
    m_state->update(*this, deltaTime);
}

void Player::draw(sf::RenderWindow& window) const {
    // CORRECTION : On passe le sprite et l'exhaust en respectant la nouvelle signature
    m_state->draw(window, m_sprite, m_exhaust);
}

void Player::collide(Object& other) {
    other.collide(*this);
}

bool Player::isDead() const { return m_isDead; }
void Player::setDead(bool dead) { m_isDead = dead; }
bool Player::isThrusting() const { return m_isThrusting; }