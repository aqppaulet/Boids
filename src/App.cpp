#include "App.h"

#include <cstdlib>

#include <GL/glut.h>

#include "Config.h"

App* App::instance_ = nullptr;

App::App()
    : mode_(Config::USE_3D ? SpaceMode::ThreeD : SpaceMode::TwoD),
      renderer_(Config::WORLD_BOUNDS, mode_) {
}

void App::run(int argc, char** argv) {
    instance_ = this;
    flock_.initialize(Config::BOID_COUNT, Config::WORLD_BOUNDS, mode_);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    glutCreateWindow("Lab 10 - Boids: estructura, poblacion y orientacion");

    renderer_.initializeOpenGL();
    renderer_.configureProjection(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutIdleFunc(idleCallback);

    glutMainLoop();
}

void App::displayCallback() {
    instance_->display();
}

void App::reshapeCallback(int width, int height) {
    instance_->reshape(width, height);
}

void App::keyboardCallback(unsigned char key, int, int) {
    instance_->keyboard(key);
}

void App::idleCallback() {
    instance_->idle();
}

void App::display() {
    renderer_.display(flock_);
}

void App::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    renderer_.configureProjection(width, height);
}

void App::keyboard(unsigned char key) {
    if (key == 27) {
        std::exit(0);
    }
}

void App::idle() {
    renderer_.updateCamera();
}
