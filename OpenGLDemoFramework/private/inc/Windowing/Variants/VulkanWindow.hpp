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

/*
* A layer can expose extensions, keep track of those
* extensions here.
*/
typedef struct {
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
} layer_properties;

class VulkanWindow : public IWindow
{
public:
    VulkanWindow(const WindowParameters& params, IApplication& app);
    virtual void startRenderLoop();
private:
    void init_instance();
    void destroy_instance();
    void init_enumerate_device();
    void init_device();
    void destroy_device();
    void init_window();
    void destroy_window();
    void init_swapchain_extension();
    void init_device_queue();
    void init_command_buffer();
    void destroy_command_buffer();
    void set_image_layout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout);
    void execute_begin_command_buffer();
    void execute_end_command_buffer();
    void execute_queue_command_buffer();
    void init_depth_buffer();
    void destroy_depth_buffer();
    bool memory_type_from_properties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex);
    void init_uniform_buffer();
    void destroy_uniform_buffer();
    void init_descriptor_and_pipeline_layouts();
    void destroy_descriptor_and_pipeline_layouts();
    void init_renderpass();
    void destroy_renderpass();
    void init_shaders();
    void destroy_shaders();
    void init_glslang();
    void finalizeGlslang();
    bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv);
    void initResources(TBuiltInResource &Resources);
    EShLanguage findLanguage(const VkShaderStageFlagBits shader_type);
    void init_framebuffers();
    void destroy_framebuffers();
    void init_vertex_buffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool use_texture);
    void destroy_vertex_buffer(const IVertexBufferObject& vbo);
    void init_descriptor_pool();
    void destroy_descriptor_pool();
    void init_pipeline_cache();
    void destroy_pipeline_cache();
    void init_pipeline();
    void destroy_pipeline();
    void draw(const IVertexBufferObject& vbo);
    void init_viewports();
    void init_scissors();
    void createFence();
    void destroyFence();
    VkResult init_global_layer_properties();
    void init_instance_extension_names();
    void init_device_extension_names();
    VkResult init_global_extension_properties(layer_properties &layer_props);
    void init_command_pool();
    void destroy_command_pool();
    void init_swap_chain();
    void destroy_swap_chain();
    void init_descriptor_set();
    void destroy_descriptor_set();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    const WindowParameters params;
    IApplication& app;

    std::vector<const char *> instance_layer_names;
    std::vector<const char *> instance_extension_names;
    std::vector<layer_properties> instance_layer_properties;
    std::vector<VkExtensionProperties> instance_extension_properties;
    VkInstance inst;

    std::vector<const char *> device_layer_names;
    std::vector<const char *> device_extension_names;
    std::vector<layer_properties> device_layer_properties;
    std::vector<VkExtensionProperties> device_extension_properties;
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
    PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
    PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
    VkDebugReportCallbackEXT debug_report_callback;
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

    struct {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo buffer_info;
    } vertex_buffer;

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
    VkSemaphore presentCompleteSemaphore;

};