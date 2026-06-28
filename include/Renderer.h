#pragma once

#include "Flock.h"

class Renderer {
public:
    Renderer(const Bounds& bounds, SpaceMode mode);

    void initializeOpenGL() const;
    void configureProjection(int width, int height) const;
    void display(const Flock& flock) const;
    void updateCamera();

private:
    bool is3D() const;
    float toDegrees(float radians) const;

    void drawBoid2D(const Boid& boid) const;
    void drawBoid3D(const Boid& boid) const;
    void drawWorldBox() const;
    void drawAxes() const;
    void configureCamera() const;

    Bounds bounds_;
    SpaceMode mode_;
    float cameraAngle_{35.0f};
};
