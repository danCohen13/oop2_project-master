#pragma once
#include "MovingGameObject.h"
#include "SpriteAnimator.h"

class Player;

class Missile : public MovingGameObject {
public:
    enum class MissileStatus { Warning, Flying };

    Missile(Player& player);
    virtual ~Missile() = default;

    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

    virtual bool isDisposed() const override { return m_isDisposed; }

private:
    Player& m_player;
    MissileStatus m_status;

    // Des animateurs spécialisés pour chaque texture
    SpriteAnimator m_missileAnimator;
    SpriteAnimator m_warningAnimator;
    SpriteAnimator m_incomingAnimator;

    mutable sf::Sprite m_warningSprite;
    float m_warningTimer;
    bool m_isDisposed;

    const float WARNING_DURATION = 2.0f;
    const float VIRTUAL_SCREEN_WIDTH = 800.0f;
};