#include "Renderer.h"

#include <cmath>
#include <cstdio>

#include <GL/glut.h>

#include "Config.h"

Renderer::Renderer(const Bounds& bounds, SpaceMode mode)
    : bounds_(bounds), mode_(mode) {
}

void Renderer::initializeOpenGL() const {
    glClearColor(0.05f, 0.06f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::configureProjection(int width, int height) {
    viewportWidth_ = width > 0 ? width : 1;
    viewportHeight_ = height > 0 ? height : 1;

    const float aspect = static_cast<float>(viewportWidth_) / static_cast<float>(viewportHeight_);

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

void Renderer::display(const Flock& flock, const SimulationSettings& settings) const {
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
            if (boid.highlightTime > 0.0f) {
                glColor3f(1.0f, 0.88f, 0.08f);
            } else {
                glColor3f(0.12f, 0.75f, 0.95f);
            }
            drawBoid2D(boid);
        }
    }

    drawHud(flock, settings);
    glutSwapBuffers();
}

void Renderer::handleMouseButton(int button, int state, int x, int y) {
    if (!is3D()) {
        return;
    }

    if (button == GLUT_LEFT_BUTTON) {
        draggingCamera_ = state == GLUT_DOWN;
        lastMouseX_ = x;
        lastMouseY_ = y;
        return;
    }

    if (state != GLUT_DOWN) {
        return;
    }

    if (button == 3) {
        cameraDistance_ = clamp(cameraDistance_ - 6.0f, 45.0f, 190.0f);
        glutPostRedisplay();
    } else if (button == 4) {
        cameraDistance_ = clamp(cameraDistance_ + 6.0f, 45.0f, 190.0f);
        glutPostRedisplay();
    }
}

void Renderer::handleMouseMove(int x, int y) {
    if (!is3D() || !draggingCamera_) {
        return;
    }

    const int deltaX = x - lastMouseX_;
    const int deltaY = y - lastMouseY_;
    lastMouseX_ = x;
    lastMouseY_ = y;

    cameraYaw_ += static_cast<float>(deltaX) * 0.35f;
    cameraPitch_ += static_cast<float>(deltaY) * 0.35f;
    cameraPitch_ = clamp(cameraPitch_, -80.0f, 80.0f);

    glutPostRedisplay();
}

bool Renderer::is3D() const {
    return mode_ == SpaceMode::ThreeD;
}

float Renderer::toDegrees(float radians) const {
    return radians * 180.0f / Config::PI;
}

float Renderer::clamp(float value, float minValue, float maxValue) const {
    if (value < minValue) {
        return minValue;
    }

    if (value > maxValue) {
        return maxValue;
    }

    return value;
}

void Renderer::configureCamera() const {
    const float yaw = cameraYaw_ * Config::PI / 180.0f;
    const float pitch = cameraPitch_ * Config::PI / 180.0f;

    const float cameraX = std::cos(pitch) * std::cos(yaw) * cameraDistance_;
    const float cameraY = std::cos(pitch) * std::sin(yaw) * cameraDistance_;
    const float cameraZ = std::sin(pitch) * cameraDistance_;

    gluLookAt(cameraX, cameraY, cameraZ,
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
    const bool highlighted = boid.highlightTime > 0.0f;

    glPushMatrix();
    glTranslatef(boid.position.x, boid.position.y, boid.position.z);
    glRotatef(yaw, 0.0f, 0.0f, 1.0f);
    glRotatef(-pitch, 0.0f, 1.0f, 0.0f);

    const float nose = Config::BOID_SIZE;
    const float tail = -Config::BOID_SIZE * 0.70f;
    const float width = Config::BOID_SIZE * 0.45f;

    glBegin(GL_TRIANGLES);
        if (highlighted) {
            glColor3f(1.0f, 0.95f, 0.12f);
        } else {
            glColor3f(0.10f, 0.90f, 1.0f);
        }
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, width, width);
        glVertex3f(tail, -width, width);

        if (highlighted) {
            glColor3f(1.0f, 0.78f, 0.05f);
        } else {
            glColor3f(0.05f, 0.56f, 0.95f);
        }
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, -width, width);
        glVertex3f(tail, -width, -width);

        if (highlighted) {
            glColor3f(0.95f, 0.58f, 0.02f);
        } else {
            glColor3f(0.02f, 0.32f, 0.78f);
        }
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, -width, -width);
        glVertex3f(tail, width, -width);

        if (highlighted) {
            glColor3f(1.0f, 0.90f, 0.20f);
        } else {
            glColor3f(0.48f, 0.98f, 1.0f);
        }
        glVertex3f(nose, 0.0f, 0.0f);
        glVertex3f(tail, width, -width);
        glVertex3f(tail, width, width);
    glEnd();

    if (highlighted) {
        glColor3f(1.0f, 0.98f, 0.55f);
    } else {
        glColor3f(0.86f, 0.96f, 1.0f);
    }
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

void Renderer::drawHud(const Flock& flock, const SimulationSettings& settings) const {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, viewportWidth_, 0.0, viewportHeight_);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0.88f, 0.94f, 1.0f);

    char text[160];
    const float x = 12.0f;
    float y = static_cast<float>(viewportHeight_) - 20.0f;
    const float lineHeight = 16.0f;

    std::snprintf(text, sizeof(text), "Boids: %zu  [1/2]", flock.boids().size());
    drawText(x, y, text);
    y -= lineHeight;

    std::snprintf(text, sizeof(text), "Vecindad: %.1f  [3/4]", settings.neighborRadius);
    drawText(x, y, text);
    y -= lineHeight;

    std::snprintf(text, sizeof(text), "Separacion: %.1f  [5/6]", settings.separationWeight);
    drawText(x, y, text);
    y -= lineHeight;

    std::snprintf(text, sizeof(text), "Alineamiento: %.1f  [7/8]", settings.alignmentWeight);
    drawText(x, y, text);
    y -= lineHeight;

    std::snprintf(text, sizeof(text), "Cohesion: %.1f  [9/0]", settings.cohesionWeight);
    drawText(x, y, text);
    y -= lineHeight;

    drawText(x, y, "Esc: salir");

    glPopAttrib();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::drawText(float x, float y, const char* text) const {
    glRasterPos2f(x, y);

    for (const char* character = text; *character != '\0'; ++character) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *character);
    }
}
