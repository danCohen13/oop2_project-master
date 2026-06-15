#pragma once
#include "LaserState.h"

class LaserRotatingState : public LaserState {
public:
    LaserRotatingState();
    void rotate(Laser* laser, float deltaTime) override;

private:
    int m_direction; // +1 ou -1
};