#pragma once
#include "StaticGameObject.h"

class PowerUpBox : public StaticGameObject {
public:
    PowerUpBox(const sf::Vector2f& position);
    virtual ~PowerUpBox() override = default;

    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

    // Surcharge pour indiquer au Board quand supprimer la boîte
    virtual bool isDisposed() const override { return m_isDisposed; }

private:
    bool m_isDisposed; // Drapeau de suppression
};