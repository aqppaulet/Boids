#pragma once

#include <cstddef>

#include "Flock.h"
#include "Renderer.h"

class App {
public:
    App();

    void run(int argc, char** argv);

private:
    static void displayCallback();
    static void reshapeCallback(int width, int height);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void mouseCallback(int button, int state, int x, int y);
    static void motionCallback(int x, int y);
    static void idleCallback();

    void display();
    void reshape(int width, int height);
    void keyboard(unsigned char key);
    void mouse(int button, int state, int x, int y);
    void motion(int x, int y);
    void idle();
    void adjustBoidCount(int delta);

    Flock flock_;
    SimulationSettings settings_;
    std::size_t boidCount_{0};
    SpaceMode mode_;
    Renderer renderer_;
    int lastUpdateTime_{0};

    static App* instance_;
};
