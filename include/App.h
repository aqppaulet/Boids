#pragma once

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
    static void idleCallback();

    void display();
    void reshape(int width, int height);
    void keyboard(unsigned char key);
    void idle();

    Flock flock_;
    SpaceMode mode_;
    Renderer renderer_;

    static App* instance_;
};
