#pragma once

#include "Vec3.h"

struct Boid {
    Vec3 position;
    Vec3 velocity;
    Vec3 direction;

    Boid() = default;

    Boid(const Vec3& initialPosition, const Vec3& initialVelocity)
        : position(initialPosition),
          velocity(initialVelocity),
          direction(initialVelocity.normalized()) {
    }
};
