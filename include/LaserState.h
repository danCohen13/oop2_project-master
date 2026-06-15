#pragma once
class Laser;

class LaserState {
public:
    virtual ~LaserState() = default;
    virtual void rotate(Laser* laser, float deltaTime) = 0;
};