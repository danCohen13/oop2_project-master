#pragma once
#include "StaticGameObject.h"
#include "LaserState.h"
#include <memory>

class Player;

class Laser : public StaticGameObject {
public:
    explicit Laser(const sf::Vector2f& position);
    virtual ~Laser() override = default;

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void update(float deltaTime) override;
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;
    virtual bool isDisposed() const override;

    void rotate(float angle);

private:
    std::unique_ptr<LaserState> m_currState;
    sf::RectangleShape m_hitbox;
    bool m_isDisposed = false;

    float m_animationTimer = 0.f;
    float m_vibrationTimer = 0.f;
    int   m_currentFrame = 0;
};