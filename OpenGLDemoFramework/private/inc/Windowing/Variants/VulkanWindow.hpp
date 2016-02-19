#pragma once

#include <Windowing/IWindow.hpp>
#include <Windowing/WindowParameters.hpp>
#include <Windowing/IApplication.hpp>

class VulkanWindow : public IWindow
{
public:
    VulkanWindow(const WindowParameters& params, IApplication& app);
    virtual void startRenderLoop();
private:
    const WindowParameters& params;
    IApplication& app;
};