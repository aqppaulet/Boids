#include "Renderer.h"

#include <cmath>

#include <GL/glut.h>

#include "Config.h"

Renderer::Renderer(const Bounds& bounds, SpaceMode mode)
    : bounds_(bounds), mode_(mode) {
}

void Renderer::initializeOpenGL() const {
    glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::configureProjection(int width, int height) const {
    const float aspect = static_cast<float>(width) / static_cast<float>(height == 0 ? 1 : height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (is3D()) {
        gluPerspective(60.0, aspect, 1.0, 300.0);
    } else {
        const float halfHeight = 40.0f;
        const float halfWidth = halfHeight * aspect;
        gluOrtho2D(-halfWidth, halfWidth, -halfHeight, halfHeight);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::display(const Flock& flock) const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (is3D()) {
        configureCamera();
        drawWorldBox();
        drawAxes();
    }

    for (const Boid& boid : flock.boids()) {
        if (is3D()) {
            drawBoid3D(boid);
        } else {
            glColor3f(0.12f, 0.75f, 0.95f);
            drawBoid2D(boid);
        }
    }

    glutSwapBuffers();
}

void Renderer::updateCamera() {
    if (!is3D()) {
        return;
    }

    cameraAngle_ += 0.08f;
    if (cameraAngle_ >= 360.0f) {
        cameraAngle_ -= 360.0f;
    }

    glutPostRedisplay();
}

bool Renderer::is3D() const {
    return mode_ == SpaceMode::ThreeD;
}

float Renderer::toDegrees(float radians) const {
    return radians * 180.0f / Config::PI;
}

void Renderer::configureCamera() const {
    const float angle = cameraAngle_ * Config::PI / 180.0f;
    const float cameraX = std::cos(angle) * 95.0f;
    const float cameraY = std::sin(angle) * 95.0f;

    gluLookAt(cameraX, cameraY, 70.0,
              0.0, 0.0, 0.0,
              0.0, 0.0, 1.0);
}

void Renderer::drawBoid2D(const Boid& boid) const {
    const float angle = toDegrees(std::atan2(boid.direction.y, boid.direction.x));

    glPushMatrix();
    glTranslatef(boid.position.x, boid.position.y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(Config::BOID_SIZE, 0.0f);
        glVertex2f(-Config::BOID_SIZE * 0.65f, Config::BOID_SIZE * 0.45f);
        glVertex2f(-Config::BOID_SIZE * 0.65f, -Config::BOID_SIZE * 0.45f);
    glEnd();

    glPopMatrix();
}

void Renderer::drawBoid3D(const Boid& boid) const {
    const Vec3 direction = boid.direction.normalized();
    const float yaw = toDegrees(std::atan2(direction.y, direction.x));
    const float xyLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    const float pitch = toDegrees(std::atan2(direction.z, xyLength));

    glPushMatrix();
    glTranslatef(boid.position.x, boid.position.y, boid.position.z);
    glRotatef(yaw, 0.0f, 0.0f, 1.0f);
    glRotatef(-pitch, 0.0f, 1.0f, 0.0f);

    const float nose = Config::BOID_SIZE;
    const float tail = -Config::BOID_SIZE * 0.70f;
    const float width = Config::BOID_SIZE * 0.45f;

    glBegin(GL_TRIANGLES);
        glColor3f(0.10f, 0.90f, 1.0f);
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, width, width);
        glVertex3f(tail, -width, width);

        glColor3f(0.05f, 0.56f, 0.95f);
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, -width, width);
        glVertex3f(tail, -width, -width);

        glColor3f(0.02f, 0.32f, 0.78f);
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, -width, -width);
        glVertex3f(tail, width, -width);

        glColor3f(0.48f, 0.98f, 1.0f);
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, width, -width);
        glVertex3f(tail, width, width);
    glEnd();

    glColor3f(0.86f, 0.96f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(tail, width, width);
        glVertex3f(tail, -width, width);
        glVertex3f(tail, -width, -width);
        glVertex3f(tail, width, -width);
    glEnd();

    glPopMatrix();
}

void Renderer::drawWorldBox() const {
    glColor3f(0.32f, 0.36f, 0.42f);
    glBegin(GL_LINES);
        glVertex3f(bounds_.minX, bounds_.minY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.minX, bounds_.minY, bounds_.minZ);

        glVertex3f(bounds_.minX, bounds_.minY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.maxZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.maxZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.maxZ);
        glVertex3f(bounds_.minX, bounds_.minY, bounds_.maxZ);

        glVertex3f(bounds_.minX, bounds_.minY, bounds_.minZ);
        glVertex3f(bounds_.minX, bounds_.minY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.minY, bounds_.maxZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.maxX, bounds_.maxY, bounds_.maxZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.minZ);
        glVertex3f(bounds_.minX, bounds_.maxY, bounds_.maxZ);
    glEnd();
}

void Renderer::drawAxes() const {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.22f, 0.18f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(58.0f, 0.0f, 0.0f);

        glColor3f(0.25f, 0.95f, 0.35f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 43.0f, 0.0f);

        glColor3f(0.38f, 0.62f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 33.0f);
    glEnd();
    glLineWidth(1.0f);
}
