#include "Windowing/Variants/VulkanWindow.hpp"
#include <iostream>
#include <cassert>
#include <cstdlib>
#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#define FENCE_TIMEOUT 100000000

VulkanWindow::VulkanWindow(const WindowParameters & params, IApplication & app) : params(params), app(app)
{

}

void VulkanWindow::startRenderLoop()
{
    createInstance();
    enumerateGPUs();
    createDevice();
    initWindow();
    initSwapChain();
    createCommandBuffer();
    execute_begin_command_buffer();
    createImageViews();
    execute_end_command_buffer();
    execute_queue_command_buffer();
    destroyImageViews();
    destroyCommandBuffer();
    destroySwapChain();
    destroyWindow();
    destroyDevice();
    destroyInstance();
}

void VulkanWindow::createInstance()
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
}

void VulkanWindow::destroyInstance()
{
    vkDestroyInstance(inst, NULL);
}

void VulkanWindow::enumerateGPUs()
{
    VkResult res;
    uint32_t gpu_count = 1;
    res = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);
    assert(gpu_count);
    
    gpus.resize(gpu_count);
    res = vkEnumeratePhysicalDevices(inst, &gpu_count, gpus.data());
    assert(!res && gpu_count >= 1);
}

void VulkanWindow::createDevice()
{
    VkResult res;
    VkDeviceQueueCreateInfo queue_info = {};

    vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queue_count, NULL);
    assert(queue_count >= 1);

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

    res = vkCreateDevice(gpus[0], &device_info, NULL, &device);
    assert(res == VK_SUCCESS);
}

void VulkanWindow::destroyDevice()
{
    vkDestroyDevice(device, NULL);
}

// MS-Windows event handling function:
LRESULT CALLBACK VulkanWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    VulkanWindow* window = reinterpret_cast<VulkanWindow*>(
        GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        window->app.onUpdate(0);
        window->app.onRender(0);
        return 0;
    default:
        break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void VulkanWindow::initWindow() {
    WNDCLASSEX win_class;

    connection = GetModuleHandle(NULL);

    // Initialize the window class structure:
    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = VulkanWindow::WndProc;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = connection; // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = params.name.c_str();
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        printf("Unexpected error trying to start the application!\n");
        fflush(stdout);
        exit(1);
    }
    // Create window with the registered class:
    RECT wr = { 0, 0, params.width, params.height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    window = CreateWindowEx(0,
        params.name.c_str(),            // class name
        params.name.c_str(),            // app name
        WS_OVERLAPPEDWINDOW |           // window style
        WS_VISIBLE | WS_SYSMENU,
        params.posX, params.posY,       // x/y coords
        params.width,       // width
        params.height,      // height
        NULL,               // handle to parent
        NULL,               // handle to menu
        connection,         // hInstance
        NULL);              // no extra parameters
    if (!window) {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR) this);
}

void VulkanWindow::destroyWindow() {
    vkDestroySurfaceKHR(inst, surface, NULL);
    DestroyWindow(window);
}

void VulkanWindow::initSwapChain()
{
    VkResult U_ASSERT_ONLY res;
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = connection;
    createInfo.hwnd = window;
    res = vkCreateWin32SurfaceKHR(inst, &createInfo, NULL, &surface);
    assert(res == VK_SUCCESS);

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32* supportsPresent = new VkBool32[queue_count];
    for (uint32_t i = 0; i < queue_count; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &supportsPresent[i]);
    }

    // Search for a graphics queue and a present queue in the array of queue
    // families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queue_count; i++) {
        if ((queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            if (supportsPresent[i] == VK_TRUE) {
                graphicsQueueNodeIndex = i;
                break;
            }
        }
    }
    
    delete[] supportsPresent;

    // Generate error if could not find a queue that supports both a graphics
    // and present
    if (graphicsQueueNodeIndex == UINT32_MAX) {
        std::cout << "Could not find a queue that supports both graphics and "
            "present\n";
        exit(-1);
    }

    graphics_queue_family_index = graphicsQueueNodeIndex;

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, NULL);
    assert(res == VK_SUCCESS);
    VkSurfaceFormatKHR* surfFormats = new VkSurfaceFormatKHR[formatCount];
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, surfFormats);
    assert(res == VK_SUCCESS);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        format = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else {
        assert(formatCount >= 1);
        format = surfFormats[0].format;
    }

    delete[] surfFormats;

    VkSurfaceCapabilitiesKHR surfCapabilities;

    res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[0], surface, &surfCapabilities);
    assert(res == VK_SUCCESS);

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, NULL);
    assert(res == VK_SUCCESS);
    VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, presentModes);
    assert(res == VK_SUCCESS);

    VkExtent2D swapChainExtent;
    // width and height are either both -1, or both not -1.
    if (surfCapabilities.currentExtent.width == (uint32_t) -1) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapChainExtent.width = params.width;
        swapChainExtent.height = params.height;
    }
    else {
        // If the surface size is defined, the swap chain size must match
        swapChainExtent = surfCapabilities.currentExtent;
    }

    // If mailbox mode is available, use it, as is the lowest-latency non-
    // tearing mode.  If not, try IMMEDIATE which will usually be available,
    // and is fastest (though it tears).  If not, fall back to FIFO which is
    // always available.
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
            (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    delete[] presentModes;

    // Determine the number of VkImage's to use in the swap chain (we desire to
    // own only 1 image at a time, besides the images being displayed and
    // queued for display):
    uint32_t desiredNumberOfSwapChainImages =
        surfCapabilities.minImageCount + 1;
    if ((surfCapabilities.maxImageCount > 0) &&
        (desiredNumberOfSwapChainImages > surfCapabilities.maxImageCount)) {
        // Application must settle for fewer images than desired:
        desiredNumberOfSwapChainImages = surfCapabilities.maxImageCount;
    }

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms &
        VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swap_chain_req = {};
    swap_chain_req.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_req.pNext = NULL;
    swap_chain_req.surface = surface;
    swap_chain_req.minImageCount = desiredNumberOfSwapChainImages;
    swap_chain_req.imageFormat = format;
    swap_chain_req.imageExtent.width = swapChainExtent.width;
    swap_chain_req.imageExtent.height = swapChainExtent.height;
    swap_chain_req.preTransform = preTransform;
    swap_chain_req.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_chain_req.imageArrayLayers = 1;
    swap_chain_req.presentMode = swapchainPresentMode;
    swap_chain_req.oldSwapchain = NULL;
    swap_chain_req.clipped = true;
    swap_chain_req.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swap_chain_req.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swap_chain_req.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swap_chain_req.queueFamilyIndexCount = 0;
    swap_chain_req.pQueueFamilyIndices = NULL;

    res = vkCreateSwapchainKHR(device, &swap_chain_req, NULL, &swap_chain);
    assert(res == VK_SUCCESS);

    res = vkGetSwapchainImagesKHR(device, swap_chain, &swapchainImageCount, NULL);
    assert(res == VK_SUCCESS);

    VkImage* swapchainImages = new VkImage[swapchainImageCount];
    assert(swapchainImages);
    res = vkGetSwapchainImagesKHR(device, swap_chain, &swapchainImageCount, swapchainImages);
    assert(res == VK_SUCCESS);

    buffers.resize(swapchainImageCount);

    for (uint32_t i = 0; i < swapchainImageCount; i++) 
    {
        buffers[i].image = swapchainImages[i];
    }

    delete[] swapchainImages;
}

void VulkanWindow::destroySwapChain()
{
    vkDestroySwapchainKHR(device, swap_chain, NULL);
}

void VulkanWindow::createCommandBuffer()
{
    VkResult U_ASSERT_ONLY res;
    /* Create a command pool to allocate our command buffer from */
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = graphics_queue_family_index;
    cmd_pool_info.flags = 0;

    res = vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmd_pool);
    assert(res == VK_SUCCESS);

    /* Create the command buffer from the command pool */
    VkCommandBufferAllocateInfo cmd_req = {};
    cmd_req.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_req.pNext = NULL;
    cmd_req.commandPool = cmd_pool;
    cmd_req.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd_req.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(device, &cmd_req, &cmd);
    assert(res == VK_SUCCESS);
}

void VulkanWindow::destroyCommandBuffer()
{
    VkCommandBuffer cmd_bufs[1] = { cmd };
    vkFreeCommandBuffers(device, cmd_pool, 1, cmd_bufs);
    vkDestroyCommandPool(device, cmd_pool, NULL);
}

void VulkanWindow::set_image_layout(VkImage image, VkImageAspectFlags aspectMask,  VkImageLayout old_image_layout, VkImageLayout new_image_layout)
{
    /* DEPENDS on info.cmd and info.queue initialized */
    assert(cmd != VK_NULL_HANDLE);
    assert(queue != VK_NULL_HANDLE);

    VkImageMemoryBarrier image_memory_barrier = {};
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_memory_barrier.pNext = NULL;
    image_memory_barrier.srcAccessMask = 0;
    image_memory_barrier.dstAccessMask = 0;
    image_memory_barrier.oldLayout = old_image_layout;
    image_memory_barrier.newLayout = new_image_layout;
    image_memory_barrier.image = image;
    image_memory_barrier.subresourceRange.aspectMask = aspectMask;
    image_memory_barrier.subresourceRange.baseMipLevel = 0;
    image_memory_barrier.subresourceRange.levelCount = 1;
    image_memory_barrier.subresourceRange.layerCount = 1;

    if (old_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.srcAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }

    if (old_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }

    if (old_image_layout == VK_IMAGE_LAYOUT_PREINITIALIZED) {
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        image_memory_barrier.srcAccessMask =
            VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmd, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, &image_memory_barrier);
}

void VulkanWindow::execute_begin_command_buffer()
{
    /* DEPENDS on init_command_buffer() */
    VkResult U_ASSERT_ONLY res;

    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;

    res = vkBeginCommandBuffer(cmd, &cmd_buf_info);
    assert(res == VK_SUCCESS);
}

void VulkanWindow::execute_end_command_buffer()
{
    VkResult U_ASSERT_ONLY res;

    res = vkEndCommandBuffer(cmd);
    assert(res == VK_SUCCESS);
}

void VulkanWindow::createImageViews()
{
    VkResult U_ASSERT_ONLY res;

    vkGetDeviceQueue(device, graphics_queue_family_index, 0, &queue);

    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.format = format;
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.flags = 0;

        set_image_layout(buffers[i].image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        color_image_view.image = buffers[i].image;

        res = vkCreateImageView(device, &color_image_view, NULL, &buffers[i].view);
        assert(res == VK_SUCCESS);
    }
}

void VulkanWindow::destroyImageViews()
{
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        vkDestroyImageView(device, buffers[i].view, NULL);
    }
}

void VulkanWindow::execute_queue_command_buffer()
{
    VkResult U_ASSERT_ONLY res;

    /* Queue the command buffer for execution */
    const VkCommandBuffer cmd_bufs[] = { cmd };
    VkFenceCreateInfo fenceInfo;
    VkFence drawFence;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vkCreateFence(device, &fenceInfo, NULL, &drawFence);

    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 0;
    submit_info[0].pWaitSemaphores = NULL;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    res = vkQueueSubmit(queue, 1, submit_info, drawFence);
    assert(res == VK_SUCCESS);

    do {
        res = vkWaitForFences(device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
    } while (res == VK_TIMEOUT);
    assert(res == VK_SUCCESS);

    vkDestroyFence(device, drawFence, NULL);
}
