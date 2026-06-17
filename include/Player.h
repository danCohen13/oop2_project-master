#pragma once
#include "MovingGameObject.h"
#include "Exhaust.h"
#include <memory>

class PlayerState;

class Player : public MovingGameObject {
public:
    // Ajout de SuperPowerRunner dans l'énumération
    enum class MovementType { Walking, Jumping, Dying, Boosting, SuperPowerRunner };

    Player();
    virtual ~Player() = default;

    void update(float deltaTime, bool isThrusting);
    virtual void update(float deltaTime) override { update(deltaTime, false); }

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player&) override {}

    bool isDead()      const;
    void setDead(bool dead);
    bool isThrusting() const;

    // Interface épurée du Boost
    void activateSpeedBoost(float distanceInPixels);
    bool isSpeedBoosting() const { return m_currentMovement == MovementType::Boosting; }
    void stopBoost();

    // Contrôle d'invincibilité externe
    void setInvincible(bool invincible) { m_isInvincible = invincible; }
    bool isInvincible() const { return m_isInvincible; }

    // Pouvoir SuperPowerRunner
    void activateSuperPowerRunner();
    bool isSuperPowerRunner() const { return m_currentMovement == MovementType::SuperPowerRunner; }

    // CORRECTION : Accesseurs requis pour les calculs physiques de l'état
    float getFloorY() const { return m_floorY; }
    float getCeilingY() const { return CEILING_Y; }

    sf::Sprite& getSprite() { return m_sprite; }
    Exhaust& getExhaust() { return m_exhaust; }

private:
    float m_verticalVelocity;
    bool  m_isDead;
    bool  m_isThrusting;
    bool  m_isInvincible;

    Exhaust m_exhaust;

    std::unique_ptr<PlayerState> m_state;
    MovementType m_currentMovement;

    const float GRAVITY = 980.f;
    const float JETPACK_FORCE = -600.f;
    const float CEILING_Y = 50.f;
    float m_floorY;
};