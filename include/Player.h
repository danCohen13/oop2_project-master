#pragma once
#include "MovingGameObject.h"
#include "Exhaust.h"
#include <memory>

class PlayerState;

class Player : public MovingGameObject {
public:
    Player();
    virtual ~Player() override = default;

    void update(float deltaTime, bool isThrusting);
    virtual void update(float deltaTime) override { update(deltaTime, false); }

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player&) override {}

    bool isDead() const;
    void setDead(bool dead);
    bool isThrusting() const;
    bool isSpeedBoosting() const;
    bool isSuperPowerRunner() const;

    void setInvincible(bool invincible) { m_isInvincible = invincible; }
    bool isInvincible() const;

    void changeState(std::unique_ptr<PlayerState> newState);

    void activateSpeedBoost(float distanceInPixels);
    void stopBoost();
    void activateSuperPowerRunner();

    void applyStandardPhysics(float deltaTime, bool useJetpack);

    float getFloorY() const { return m_floorY; }
    float getCeilingY() const { return CEILING_Y; }
    float getVerticalVelocity() const { return m_verticalVelocity; }
    void setVerticalVelocity(float velocity) { m_verticalVelocity = velocity; }

    sf::Sprite& getSprite() { return m_sprite; }
    const sf::Sprite& getSprite() const { return m_sprite; }
    Exhaust& getExhaust() { return m_exhaust; }

private:
    float m_verticalVelocity;
    bool  m_isThrusting;
    bool  m_isInvincible;

    Exhaust m_exhaust;
    std::unique_ptr<PlayerState> m_state;

    const float GRAVITY = 980.f;
    const float JETPACK_FORCE = -600.f;
    const float CEILING_Y = 50.f;
    float m_floorY;
};