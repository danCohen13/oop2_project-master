#pragma once
#include "Screen.h"

class MenuScreen : public Screen {
public:
    MenuScreen(ScreenStack& stack);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;
};