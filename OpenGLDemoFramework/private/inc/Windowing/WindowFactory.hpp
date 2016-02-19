#pragma once

#include <Windowing/WindowParameters.hpp>
#include <Windowing/IWindow.hpp>
#include <Windowing/IApplication.hpp>

class WindowFactory
{
public:
    IWindow* createWindow(const WindowParameters& params, IApplication& app);
};