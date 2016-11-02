#include "Demos/Demo3DBase.hpp"

Demo3DBase::Demo3DBase(const Vec2 resolution) :
    resolution(resolution),
    camera(3.0f / 4.0f, resolution.x / resolution.y, 1.0f, 1000.0f),
    renderer(0)
{

}

Demo3DBase::~Demo3DBase()
{
}

void Demo3DBase::onInit()
{
    renderer = new Renderer(resolution);

    initTextures();
    initMaterials();
    initGeometry();

    time = 0;
    stopTime = false;
    prevMousePos = Vec2(0.5, 0.5);
    phi = 3.14f / 2.0f;
    theta = 0;
    radius = 30;
    float t = radius * cos(theta);
    cameraPos = Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
    camera.setPosition(cameraPos);
}

void Demo3DBase::onUpdate(const unsigned int deltaTime)
{
}

void Demo3DBase::onRender(const unsigned int deltaTime)
{
    if (!stopTime)
    {
        time++;
    }

    camera.setPosition(cameraPos);
}

void Demo3DBase::onDestroy()
{
    delete renderer;
}

void Demo3DBase::onEvent(const unsigned int event)
{
    cout << "onEvent: " << event << endl;
}

void Demo3DBase::onMouseEvent(int button, int state, int x, int y)
{
    cout << "onMouseEvent: " << button << " " << state << endl;
    if (0 == button)
    {
        cameraRotating = (0 == state ? true : false);
    }

    if (2 == button)
    {
        cameraPanning = (0 == state ? true : false);
    }

    if (3 == button)
    {
        radius -= 1.0f;
        updateCamera();
    }

    if (4 == button)
    {
        radius += 1.0f;
        updateCamera();
    }
}

void Demo3DBase::onKeyboardEvent(unsigned char c, int x, int y)
{
    cout << static_cast<int>(c) << " " << x << " " << y << endl;
    stopTime = !stopTime;
}

void Demo3DBase::onMouseMove(int x, int y)
{
    Vec2 delta = Vec2(static_cast<float>(x), static_cast<float>(y)) - prevMousePos;
    delta.x /= resolution.x;
    delta.y /= resolution.y;
    if (cameraRotating)
    {
        phi += 3.14f * delta.x;
        theta += 3.14f * delta.y;
        updateCamera();
    }

    if (cameraPanning)
    {
        Vec3 center = camera.getLookDirection();
        Vec3 z = center - cameraPos;
        Vec3 y = camera.getUpVector();
        Vec3 dx = z.cross(y).normalize();
        Vec3 dy = z.cross(dx).normalize();

        cameraPos += (dx * delta.x + dy * delta.y) * 30.0f;
        camera.setLookDirection(center + (dx * delta.x + dy * delta.y) * 30.0f);
    }

    prevMousePos = Vec2(static_cast<float>(x), static_cast<float>(y));
}

void Demo3DBase::initTextures()
{
}

void Demo3DBase::initMaterials()
{
}

void Demo3DBase::initGeometry()
{
}

void Demo3DBase::updateCamera()
{
    float t = radius * cos(theta);
    cameraPos = camera.getLookDirection() + Vec3(t * cos(phi), radius * sin(theta), t * sin(phi));
}
