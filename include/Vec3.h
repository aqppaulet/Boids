#pragma once

#include <cmath>

struct Vec3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    Vec3() = default;

    Vec3(float px, float py, float pz = 0.0f)
        : x(px), y(py), z(pz) {
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }

    Vec3 normalized() const {
        const float len = length();
        if (len <= 0.00001f) {
            return Vec3(1.0f, 0.0f, 0.0f);
        }

        return Vec3(x / len, y / len, z / len);
    }
};
