#include <Windowing/WindowFactory.hpp>
#include <Windowing/Variants/VulkanWindow.hpp>

IWindow* WindowFactory::createWindow(const WindowParameters& params, IApplication& app)
{
    return new VulkanWindow(params, app);
}