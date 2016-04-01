#include "Windowing/Variants/VulkanWindow.hpp"
#include <iostream>
#include <cassert>
#include <cstdlib>
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
    createInstance();
    enumerateGPUs();
    createDevice();
    initWindow();
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

void VulkanWindow::createCommandBuffer()
{

}

void VulkanWindow::destroyCommandBuffer()
{
}
