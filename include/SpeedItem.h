#pragma once
#include "StaticGameObject.h"

class SpeedItem : public StaticGameObject {
public:
    SpeedItem(const sf::Vector2f& position);
    virtual ~SpeedItem() override = default;

    // Implémentation obligatoire des fonctions virtuelles pures de Object
    virtual void draw(sf::RenderWindow& window) const override;
    virtual void collide(Object& other) override;
    virtual void collide(Player& player) override;

    // Surcharge pour indiquer au Board quand le détruire de la mémoire
    virtual bool isDisposed() const override;

private:
    bool m_isDisposed = false; // Résout l'erreur C2065
};