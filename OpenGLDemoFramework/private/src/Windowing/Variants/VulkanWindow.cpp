#include "Windowing/Variants/VulkanWindow.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

VulkanWindow::VulkanWindow(const WindowParameters & params, IApplication & app) : params(params), app(app)
{

}

void VulkanWindow::startRenderLoop()
{
    // initialize the VkApplicationInfo structure
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = params.name.c_str();
    app_info.applicationVersion = 1;
    app_info.pEngineName = params.name.c_str();
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    // initialize the VkInstanceCreateInfo structure
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;

    VkInstance inst;
    VkResult res;

    res = vkCreateInstance(&inst_info, NULL, &inst);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    }
    else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    }

    uint32_t gpu_count = 1;
    res = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);
    assert(gpu_count);
    std::vector<VkPhysicalDevice> gpus;
    gpus.resize(gpu_count);
    res = vkEnumeratePhysicalDevices(inst, &gpu_count, gpus.data());
    assert(!res && gpu_count >= 1);

    VkDeviceQueueCreateInfo queue_info = {};
    uint32_t queue_count;

    vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queue_count, NULL);
    assert(queue_count >= 1);

    std::vector<VkQueueFamilyProperties> queue_props;
    queue_props.resize(queue_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queue_count, queue_props.data());
    assert(queue_count >= 1);

    bool found = false;
    for (unsigned int i = 0; i < queue_count; i++) {
        if (queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queue_info.queueFamilyIndex = i;
            found = true;
            break;
        }
    }
    assert(found);
    assert(queue_count >= 1);

    float queue_priorities[1] = { 0.0 };
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = NULL;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = queue_priorities;

    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledExtensionCount = 0;
    device_info.ppEnabledExtensionNames = NULL;
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;
    device_info.pEnabledFeatures = NULL;

    VkDevice device;
    res = vkCreateDevice(gpus[0], &device_info, NULL, &device);
    assert(res == VK_SUCCESS);

    vkDestroyDevice(device, NULL);

    vkDestroyInstance(inst, NULL);
}
