#pragma once

#include "Vec3.h"

struct Boid {
    Vec3 position;
    Vec3 velocity;
    Vec3 direction;
    float highlightTime{0.0f};

    Boid() = default;

    Boid(const Vec3& initialPosition, const Vec3& initialVelocity, float initialHighlightTime = 0.0f)
        : position(initialPosition),
          velocity(initialVelocity),
          direction(initialVelocity.normalized()),
          highlightTime(initialHighlightTime) {
    }
};
