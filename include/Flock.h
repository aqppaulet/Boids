#pragma once

#include <cstddef>
#include <vector>

#include "Boid.h"

enum class SpaceMode {
    TwoD,
    ThreeD
};

struct Bounds {
    float minX{-50.0f};
    float maxX{50.0f};
    float minY{-35.0f};
    float maxY{35.0f};
    float minZ{0.0f};
    float maxZ{0.0f};
};

class Flock {
public:
    void initialize(std::size_t count, const Bounds& bounds, SpaceMode mode);

    const std::vector<Boid>& boids() const {
        return boids_;
    }

private:
    std::vector<Boid> boids_;
};
