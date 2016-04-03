#pragma once

#include <Windowing/IWindow.hpp>
#include <Windowing/WindowParameters.hpp>
#include <Windowing/IApplication.hpp>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <vector>
#include <map>
#include "SPIRV/GlslangToSpv.h"
#include "Core/IVertexBufferObject.hpp"

typedef struct _swap_chain_buffers {
    VkImage image;
    VkImageView view;
} swap_chain_buffer;

struct texture_object {
    VkSampler sampler;

    VkImage image;
    VkImageLayout imageLayout;

    VkDeviceMemory mem;
    VkImageView view;
    int32_t tex_width, tex_height;
};

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
    void initDeviceQueue();
    void createCommandBuffer();
    void destroyCommandBuffer();
    void set_image_layout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout);
    void execute_begin_command_buffer();
    void execute_end_command_buffer();
    void createImageViews();
    void destroyImageViews();
    void execute_queue_command_buffer();
    void createDepthBuffer();
    void destroyDepthBuffer();
    bool memory_type_from_properties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex);
    void createUniformBuffer();
    void destroyUniformBuffer();
    void createPipelineLayout();
    void destroyPipelineLayout();
    void createDescriptorSetLayouts();
    void destroyDescriptorSetLayouts();
    void initRenderPass();
    void destroyRenderPass();
    void initShaders();
    void destroyShaders();
    void initGlslang();
    void finalizeGlslang();
    bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv);
    void initResources(TBuiltInResource &Resources);
    EShLanguage findLanguage(const VkShaderStageFlagBits shader_type);
    void initFrameBuffers();
    void destroyFrameBuffers();
    void createVertexBuffer(const IVertexBufferObject& vbo);
    void destroyVertexBuffer(const IVertexBufferObject& vbo);
    void allocDescriptorSets();
    void freeDescriptorSets();
    void initPipeline();
    void destroyPipeline();
    void draw(const IVertexBufferObject& vbo);
    void init_viewports();
    void init_scissors();
    void createFence();
    void destroyFence();

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

    struct {
        VkFormat format;

        VkImage image;
        VkDeviceMemory mem;
        VkImageView view;
    } depth;

    std::vector<texture_object> textures;

    struct {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo buffer_info;
    } uniform_data;

    struct {
        VkDescriptorImageInfo image_info;
    } texture_data;
    VkDeviceMemory stagingMemory;
    VkImage stagingImage;

    struct VertexBuffer {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo buffer_info;
    };

    VkVertexInputBindingDescription vi_binding;
    VkVertexInputAttributeDescription vi_attribs[2];
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceProperties gpu_props;
    VkFramebuffer *framebuffers;
    std::map<unsigned int, VertexBuffer> vertexBuffers;
    uint32_t current_buffer;
    VkViewport viewport;
    VkRect2D scissor;
    VkFence drawFence;
};