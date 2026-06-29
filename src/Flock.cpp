#include "Flock.h"

#include <cmath>
#include <random>
#include <vector>

namespace {
constexpr float PI = 3.1415926535f;
constexpr float EPSILON = 0.00001f;

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

Vec3 limitLength(const Vec3& vector, float maxLength) {
    if (vector.lengthSquared() <= maxLength * maxLength) {
        return vector;
    }

    return vector.normalized() * maxLength;
}

Vec3 clampSpeed(const Vec3& velocity, float minSpeed, float maxSpeed) {
    const float speed = velocity.length();

    if (speed <= EPSILON) {
        return Vec3(maxSpeed, 0.0f, 0.0f);
    }

    if (speed < minSpeed) {
        return velocity.normalized() * minSpeed;
    }

    if (speed > maxSpeed) {
        return velocity.normalized() * maxSpeed;
    }

    return velocity;
}

Vec3 steerTowards(const Boid& boid, const Vec3& desiredDirection, const SimulationSettings& settings) {
    if (desiredDirection.lengthSquared() <= EPSILON) {
        return Vec3();
    }

    const Vec3 desiredVelocity = desiredDirection.normalized() * settings.maxSpeed;
    return limitLength(desiredVelocity - boid.velocity, settings.maxForce);
}

void keep2D(Vec3& vector) {
    vector.z = 0.0f;
}

void wrapPosition(Vec3& position, const Bounds& bounds, SpaceMode mode) {
    if (position.x < bounds.minX) {
        position.x = bounds.maxX;
    } else if (position.x > bounds.maxX) {
        position.x = bounds.minX;
    }

    if (position.y < bounds.minY) {
        position.y = bounds.maxY;
    } else if (position.y > bounds.maxY) {
        position.y = bounds.minY;
    }

    if (mode == SpaceMode::ThreeD) {
        if (position.z < bounds.minZ) {
            position.z = bounds.maxZ;
        } else if (position.z > bounds.maxZ) {
            position.z = bounds.minZ;
        }
    } else {
        position.z = 0.0f;
    }
}

struct NeighborSummary {
    Vec3 separationDirection;
    Vec3 averageVelocity;
    Vec3 centerOfMass;
    int separationCount{0};
    int neighborCount{0};
};

NeighborSummary collectNeighbors(
    const std::vector<Boid>& boids,
    std::size_t currentIndex,
    const SimulationSettings& settings,
    SpaceMode mode
) {
    NeighborSummary summary;
    const Boid& boid = boids[currentIndex];

    for (std::size_t i = 0; i < boids.size(); ++i) {
        if (i == currentIndex) {
            continue;
        }

        Vec3 offset = boid.position - boids[i].position;
        if (mode == SpaceMode::TwoD) {
            keep2D(offset);
        }

        const float distance = offset.length();
        if (distance > settings.neighborRadius || distance <= EPSILON) {
            continue;
        }

        summary.averageVelocity += boids[i].velocity;
        summary.centerOfMass += boids[i].position;
        ++summary.neighborCount;

        if (distance < settings.separationRadius) {
            summary.separationDirection += offset.normalized() / distance;
            ++summary.separationCount;
        }
    }

    return summary;
}

Vec3 separationRule(const Boid& boid, const NeighborSummary& summary, const SimulationSettings& settings) {
    if (summary.separationCount == 0) {
        return Vec3();
    }

    return steerTowards(boid, summary.separationDirection, settings);
}

Vec3 alignmentRule(const Boid& boid, const NeighborSummary& summary, const SimulationSettings& settings) {
    if (summary.neighborCount == 0) {
        return Vec3();
    }

    const Vec3 averageVelocity = summary.averageVelocity / static_cast<float>(summary.neighborCount);
    return steerTowards(boid, averageVelocity, settings);
}

Vec3 cohesionRule(const Boid& boid, const NeighborSummary& summary, const SimulationSettings& settings) {
    if (summary.neighborCount == 0) {
        return Vec3();
    }

    const Vec3 centerOfMass = summary.centerOfMass / static_cast<float>(summary.neighborCount);
    return steerTowards(boid, centerOfMass - boid.position, settings);
}

Vec3 combineRules(const Boid& boid, const NeighborSummary& summary, const SimulationSettings& settings) {
    const Vec3 separation = separationRule(boid, summary, settings);
    const Vec3 alignment = alignmentRule(boid, summary, settings);
    const Vec3 cohesion = cohesionRule(boid, summary, settings);

    return separation * settings.separationWeight
        + alignment * settings.alignmentWeight
        + cohesion * settings.cohesionWeight;
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

        const float speed = randomFloat(generator, 10.0f, 18.0f);
        const Vec3 velocity = direction * speed;

        boids_.push_back(Boid(position, velocity));
    }
}

void Flock::update(float deltaTime, const Bounds& bounds, const SimulationSettings& settings, SpaceMode mode) {
    if (boids_.empty() || deltaTime <= 0.0f) {
        return;
    }

    std::vector<Vec3> nextVelocities(boids_.size());

    for (std::size_t i = 0; i < boids_.size(); ++i) {
        const Boid& boid = boids_[i];
        const NeighborSummary neighbors = collectNeighbors(boids_, i, settings, mode);
        Vec3 acceleration = combineRules(boid, neighbors, settings);

        if (mode == SpaceMode::TwoD) {
            keep2D(acceleration);
        }

        nextVelocities[i] = clampSpeed(
            boid.velocity + acceleration * deltaTime,
            settings.minSpeed,
            settings.maxSpeed
        );

        if (mode == SpaceMode::TwoD) {
            keep2D(nextVelocities[i]);
        }
    }

    for (std::size_t i = 0; i < boids_.size(); ++i) {
        Boid& boid = boids_[i];

        boid.velocity = nextVelocities[i];
        boid.position += boid.velocity * deltaTime;
        wrapPosition(boid.position, bounds, mode);
        boid.direction = boid.velocity.normalized();
    }
}
