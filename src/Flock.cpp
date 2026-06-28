#include "Flock.h"

#include <cmath>
#include <random>

namespace {
constexpr float PI = 3.1415926535f;

float randomFloat(std::mt19937& generator, float minValue, float maxValue) {
    std::uniform_real_distribution<float> distribution(minValue, maxValue);
    return distribution(generator);
}

Vec3 randomDirection2D(std::mt19937& generator) {
    const float angle = randomFloat(generator, 0.0f, 2.0f * PI);
    return Vec3(std::cos(angle), std::sin(angle), 0.0f).normalized();
}

Vec3 randomDirection3D(std::mt19937& generator) {
    const float angle = randomFloat(generator, 0.0f, 2.0f * PI);
    const float z = randomFloat(generator, -1.0f, 1.0f);
    const float radius = std::sqrt(1.0f - z * z);

    return Vec3(radius * std::cos(angle), radius * std::sin(angle), z).normalized();
}
}

void Flock::initialize(std::size_t count, const Bounds& bounds, SpaceMode mode) {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    boids_.clear();
    boids_.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        const Vec3 position(
            randomFloat(generator, bounds.minX, bounds.maxX),
            randomFloat(generator, bounds.minY, bounds.maxY),
            mode == SpaceMode::ThreeD ? randomFloat(generator, bounds.minZ, bounds.maxZ) : 0.0f
        );

        const Vec3 direction = mode == SpaceMode::ThreeD
            ? randomDirection3D(generator)
            : randomDirection2D(generator);

        const float speed = randomFloat(generator, 0.20f, 0.80f);
        const Vec3 velocity = direction * speed;

        boids_.push_back(Boid(position, velocity));
    }
}
