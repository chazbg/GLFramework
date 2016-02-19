#pragma once

class IApplication
{
public:
    virtual ~IApplication() {}
    virtual void onInit() = 0;
    virtual void onUpdate(const unsigned int deltaTime) = 0;
    virtual void onRender(const unsigned int deltaTime) = 0;
    virtual void onDestroy() = 0;
    virtual void onEvent(const unsigned int event) = 0;
    virtual void onMouseEvent(int button, int state, int x, int y) = 0;
    virtual void onKeyboardEvent(unsigned char c, int x, int y) = 0;
    virtual void onMouseMove(int x, int y) = 0;
};