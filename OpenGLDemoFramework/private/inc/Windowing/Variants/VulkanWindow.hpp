#pragma once

#include <Windowing/IWindow.hpp>
#include <Windowing/WindowParameters.hpp>
#include <Windowing/IApplication.hpp>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <vector>

typedef struct _swap_chain_buffers {
    VkImage image;
    VkImageView view;
} swap_chain_buffer;

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
    void initWindow();
    void destroyWindow();
    void initSwapChain();
    void destroySwapChain();
    void createCommandBuffer();
    void destroyCommandBuffer();
    void set_image_layout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout);
    void execute_begin_command_buffer();
    void execute_end_command_buffer();
    void createImageViews();
    void destroyImageViews();
    void execute_queue_command_buffer();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    const WindowParameters params;
    IApplication& app;
    VkInstance inst;
    std::vector<VkPhysicalDevice> gpus;
    VkDevice device;
    VkQueue queue;
    HINSTANCE connection;        // hInstance - Windows Instance
    HWND window;                 // hWnd - window handle
    VkSurfaceKHR surface;
    uint32_t queue_count;
    std::vector<VkQueueFamilyProperties> queue_props;
    uint32_t graphics_queue_family_index;
    VkFormat format;
    uint32_t swapchainImageCount;
    VkSwapchainKHR swap_chain;
    std::vector<swap_chain_buffer> buffers;
    VkCommandPool cmd_pool;
    VkCommandBuffer cmd; // Buffer for initialization commands
    VkPipelineLayout pipeline_layout;
    std::vector<VkDescriptorSetLayout> desc_layout;
    VkPipelineCache pipelineCache;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkDescriptorPool desc_pool;
    std::vector<VkDescriptorSet> desc_set;
};