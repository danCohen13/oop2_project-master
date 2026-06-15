#pragma once
#include "LaserState.h"

class LaserStaticState : public LaserState {
public:
    void rotate(Laser* laser, float deltaTime) override {} // immobile
};