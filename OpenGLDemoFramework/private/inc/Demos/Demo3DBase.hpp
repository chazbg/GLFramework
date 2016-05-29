#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Core/PerspectiveCamera.hpp"

class Demo3DBase : public IApplication
{
public:
    Demo3DBase(const Vec2 resolution);
    virtual ~Demo3DBase();
    virtual void onInit();
    virtual void onUpdate(const unsigned int deltaTime);
    virtual void onRender(const unsigned int deltaTime);
    virtual void onDestroy();
    virtual void onEvent(const unsigned int event);
    virtual void onMouseEvent(int button, int state, int x, int y);
    virtual void onKeyboardEvent(unsigned char c, int x, int y);
    virtual void onMouseMove(int x, int y);
protected:
    virtual void initTextures();
    virtual void initMaterials();
    virtual void initGeometry();
    virtual void updateCamera();
    Vec2 resolution;
    PerspectiveCamera camera;
    Scene scene;
    Renderer* renderer;
    unsigned int time;
    bool stopTime;
    Vec3 cameraPos;
    Vec3 prevDir;
    Vec2 prevMousePos;
    float phi;
    float theta;
    float radius;
    bool cameraRotating;
    bool cameraPanning;
};