#pragma once

#include <Windowing/IWindow.hpp>
#include <Windowing/WindowParameters.hpp>
#include <Windowing/IApplication.hpp>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <vector>

class VulkanWindow : public IWindow
{
public:
    VulkanWindow(const WindowParameters& params, IApplication& app);
    virtual void startRenderLoop();
private:
    void createInstance();
    void destroyInstance();
    void enumerateGPUs();
    void createDevice();
    void destroyDevice();
    void createCommandBuffer();
    void destroyCommandBuffer();
    void initWindow();
    void destroyWindow();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    const WindowParameters params;
    IApplication& app;
    VkInstance inst;
    std::vector<VkPhysicalDevice> gpus;
    VkDevice device;
    HINSTANCE connection;        // hInstance - Windows Instance
    HWND window;                 // hWnd - window handle
    VkSurfaceKHR surface;
};