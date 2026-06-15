#include "LaserRotatingState.h"
#include "Laser.h"
#include <cstdlib>

LaserRotatingState::LaserRotatingState()
    : m_direction((rand() % 2 == 0) ? 1 : -1) {
}

void LaserRotatingState::rotate(Laser* laser, float deltaTime) {
    laser->rotate(m_direction * 90.0f * deltaTime); // 90°/sec
}