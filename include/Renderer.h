#pragma once

#include "Flock.h"

class Renderer {
public:
    Renderer(const Bounds& bounds, SpaceMode mode);

    void initializeOpenGL() const;
    void configureProjection(int width, int height);
    void display(const Flock& flock, const SimulationSettings& settings) const;
    void handleMouseButton(int button, int state, int x, int y);
    void handleMouseMove(int x, int y);

private:
    bool is3D() const;
    float toDegrees(float radians) const;
    float clamp(float value, float minValue, float maxValue) const;

    void drawBoid2D(const Boid& boid) const;
    void drawBoid3D(const Boid& boid) const;
    void drawWorldBox() const;
    void drawAxes() const;
    void drawHud(const Flock& flock, const SimulationSettings& settings) const;
    void drawText(float x, float y, const char* text) const;
    void configureCamera() const;

    Bounds bounds_;
    SpaceMode mode_;
    int viewportWidth_{900};
    int viewportHeight_{650};
    float cameraYaw_{35.0f};
    float cameraPitch_{34.0f};
    float cameraDistance_{115.0f};
    bool draggingCamera_{false};
    int lastMouseX_{0};
    int lastMouseY_{0};
};
