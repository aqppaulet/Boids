#include "App.h"

#include <cstddef>
#include <cstdlib>

#include <GL/glut.h>

#include "Config.h"

namespace {
constexpr std::size_t MIN_BOID_COUNT = 1;
constexpr std::size_t MAX_BOID_COUNT = 300;
constexpr int BOID_COUNT_STEP = 5;

float clampSetting(float value, float minValue, float maxValue) {
    if (value < minValue) {
        return minValue;
    }

    if (value > maxValue) {
        return maxValue;
    }

    return value;
}

float adjustSetting(float value, float delta, float minValue, float maxValue) {
    return clampSetting(value + delta, minValue, maxValue);
}
}

App* App::instance_ = nullptr;

App::App()
    : settings_(Config::SIMULATION),
      boidCount_(Config::BOID_COUNT),
      mode_(Config::USE_3D ? SpaceMode::ThreeD : SpaceMode::TwoD),
      renderer_(Config::WORLD_BOUNDS, mode_) {
}

void App::run(int argc, char** argv) {
    instance_ = this;
    flock_.initialize(boidCount_, Config::WORLD_BOUNDS, mode_);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    glutCreateWindow("Lab 10 - Boids: estructura, poblacion y orientacion");

    renderer_.initializeOpenGL();
    renderer_.configureProjection(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    lastUpdateTime_ = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
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

void App::mouseCallback(int button, int state, int x, int y) {
    instance_->mouse(button, state, x, y);
}

void App::motionCallback(int x, int y) {
    instance_->motion(x, y);
}

void App::idleCallback() {
    instance_->idle();
}

void App::display() {
    renderer_.display(flock_, settings_);
}

void App::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    renderer_.configureProjection(width, height);
}

void App::keyboard(unsigned char key) {
    if (key == 27) {
        std::exit(0);
    }

    switch (key) {
    case '1':
        adjustBoidCount(-BOID_COUNT_STEP);
        break;
    case '2':
        adjustBoidCount(BOID_COUNT_STEP);
        break;
    case '3':
        settings_.neighborRadius = adjustSetting(settings_.neighborRadius, -1.0f, 1.0f, 80.0f);
        break;
    case '4':
        settings_.neighborRadius = adjustSetting(settings_.neighborRadius, 1.0f, 1.0f, 80.0f);
        break;
    case '5':
        settings_.separationWeight = adjustSetting(settings_.separationWeight, -0.1f, 0.0f, 10.0f);
        break;
    case '6':
        settings_.separationWeight = adjustSetting(settings_.separationWeight, 0.1f, 0.0f, 10.0f);
        break;
    case '7':
        settings_.alignmentWeight = adjustSetting(settings_.alignmentWeight, -0.1f, 0.0f, 10.0f);
        break;
    case '8':
        settings_.alignmentWeight = adjustSetting(settings_.alignmentWeight, 0.1f, 0.0f, 10.0f);
        break;
    case '9':
        settings_.cohesionWeight = adjustSetting(settings_.cohesionWeight, -0.1f, 0.0f, 10.0f);
        break;
    case '0':
        settings_.cohesionWeight = adjustSetting(settings_.cohesionWeight, 0.1f, 0.0f, 10.0f);
        break;
    default:
        return;
    }

    glutPostRedisplay();
}

void App::mouse(int button, int state, int x, int y) {
    renderer_.handleMouseButton(button, state, x, y);
}

void App::motion(int x, int y) {
    renderer_.handleMouseMove(x, y);
}

void App::adjustBoidCount(int delta) {
    int nextCount = static_cast<int>(boidCount_) + delta;
    if (nextCount < static_cast<int>(MIN_BOID_COUNT)) {
        nextCount = static_cast<int>(MIN_BOID_COUNT);
    } else if (nextCount > static_cast<int>(MAX_BOID_COUNT)) {
        nextCount = static_cast<int>(MAX_BOID_COUNT);
    }

    boidCount_ = static_cast<std::size_t>(nextCount);
    flock_.setCount(
        boidCount_,
        Config::WORLD_BOUNDS,
        mode_,
        Config::NEW_BOID_HIGHLIGHT_SECONDS
    );
}

void App::idle() {
    const int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = static_cast<float>(currentTime - lastUpdateTime_) / 1000.0f;
    lastUpdateTime_ = currentTime;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    flock_.update(
        deltaTime,
        Config::WORLD_BOUNDS,
        settings_,
        mode_,
        Config::BOUNDARY_MODE
    );
    glutPostRedisplay();
}
