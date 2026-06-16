#pragma once
#include "MovingGameObject.h"
#include "Exhaust.h"
#include <memory>

class PlayerState;

class Player : public MovingGameObject {
public:
    enum class MovementType { Walking, Jumping, Dying, Boosting }; // Ajout de Boosting

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

    // Interface épurée pour le mode Turbo
    void activateSpeedBoost(float distanceInPixels);
    bool isSpeedBoosting() const { return m_currentMovement == MovementType::Boosting; }
    static bool isAnyPlayerBoosting() { return s_isSpeedBoosting; }
    void stopBoost(); // Appelé par BoostState pour restaurer l'état normal du joueur

    sf::Sprite& getSprite() { return m_sprite; }
    Exhaust& getExhaust() { return m_exhaust; }

private:
    float m_verticalVelocity;
    bool  m_isDead;
    bool  m_isThrusting;

    Exhaust m_exhaust;

    std::unique_ptr<PlayerState> m_state;
    MovementType m_currentMovement;

    const float GRAVITY = 980.f;
    const float JETPACK_FORCE = -600.f;
    const float CEILING_Y = 50.f;
    float m_floorY;

    static bool s_isSpeedBoosting;
};