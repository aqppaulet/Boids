#pragma once

#include "Flock.h"

namespace Config {
inline constexpr int WINDOW_WIDTH = 900;
inline constexpr int WINDOW_HEIGHT = 650;
inline constexpr int BOID_COUNT = 50;
inline constexpr float BOID_SIZE = 3.2f;
inline constexpr float PI = 3.1415926535f;

inline constexpr bool USE_3D = false;

inline const Bounds WORLD_BOUNDS{
    -50.0f, 50.0f,
    -35.0f, 35.0f,
    -25.0f, 25.0f
};
}
