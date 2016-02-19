#pragma once

class IWindow
{
public:
    virtual ~IWindow() {}
    virtual void startRenderLoop() = 0;
};
