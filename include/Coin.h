#pragma once
#include "StaticGameObject.h"

class Player;

class Coin : public StaticGameObject {
public:
    // Le constructeur obligatoire
    explicit Coin(const sf::Vector2f& position);
    virtual ~Coin() override = default;

    // 1. IMPLÉMENTATIONS DES FONCTIONS VIRTUELLES PURES OBLIGATOIRES
    virtual void draw(sf::RenderWindow& window) const override;
    virtual void update(float deltaTime) override;
    virtual void collide(Object& other) override;

    // 2. SURCHARGES POUR LE GAMEPLAY (Double Dispatch & Cycle de vie)
    virtual void collide(Player& player) override;
    virtual bool isDisposed() const override;

private:
    bool m_isDisposed = false; // Déclaration de la variable manquante
};