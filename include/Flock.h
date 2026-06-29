#pragma once

#include <cstddef>
#include <vector>

#include "Boid.h"

enum class SpaceMode {
    TwoD,
    ThreeD
};

enum class BoundaryMode {
    Bounce,
    Toroidal
};

struct Bounds {
    float minX{-50.0f};
    float maxX{50.0f};
    float minY{-35.0f};
    float maxY{35.0f};
    float minZ{0.0f};
    float maxZ{0.0f};
};

struct SimulationSettings {
    float neighborRadius{14.0f};
    float separationRadius{6.0f};

    float separationWeight{2.0f};
    float alignmentWeight{0.9f};
    float cohesionWeight{0.7f};

    float minSpeed{8.0f};
    float maxSpeed{22.0f};
    float maxForce{18.0f};
};

class Flock {
public:
    void initialize(std::size_t count, const Bounds& bounds, SpaceMode mode);
    void update(
        float deltaTime,
        const Bounds& bounds,
        const SimulationSettings& settings,
        SpaceMode mode,
        BoundaryMode boundaryMode
    );

    const std::vector<Boid>& boids() const {
        return boids_;
    }

private:
    std::vector<Boid> boids_;
};
