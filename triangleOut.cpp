#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>

#include <vector>
#include <iostream>
#include <set>


int main() {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); //принуждение GLFW к X11

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Color test", nullptr, nullptr);
    // glfwMakeContextCurrent(window); // контекст — это объект, который хранит всё состояние графической машины

    uint32_t glfwExtentionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount); // какие экстеншион-расширения нужны, чтобы работать с окнами
    std::vector<const char*> extentions(glfwExtensions, glfwExtensions + glfwExtentionCount);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = nullptr;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extentions.size());
    createInfo.ppEnabledExtensionNames = extentions.data();

    VkInstance instance = {};

    vkCreateInstance(&createInfo, nullptr, &instance);

    VkSurfaceKHR surface;
    glfwCreateWindowSurface(instance, window, nullptr, &surface);

    // Получение физической видеокарты
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr << "No Vulkan devices found!\n";
        return -1;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice physicalDevice = devices.data()[0];

    // Создание логической видеокарты
    
    uint32_t queueFamilyCount = 0; // количество потоков
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr); // получает количество потоков у видеокарты
    std::vector<VkQueueFamilyProperties>queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t graphicsQueueFamilyIndex = UINT32_MAX;
    uint32_t presentQueueFamilyIndex = UINT32_MAX;

    for(size_t i = 0; i < queueFamilies.size(); i++){
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if(presentSupport)
            presentQueueFamilyIndex = i;

        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            graphicsQueueFamilyIndex = i;
        }
    }

    if (graphicsQueueFamilyIndex == UINT32_MAX || presentQueueFamilyIndex == UINT32_MAX) {
        std::cerr << "Required queue families not found!\n";
        return -1;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    float queuePriority = 1.0f;
    

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        graphicsQueueFamilyIndex, 
        presentQueueFamilyIndex
    };

    for (const uint32_t queueFamily : uniqueQueueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

    VkDevice device;
    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);


    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window); // двойная буферизация для openGL-рендеринга. Без неё рисует в задний буфер бесконечно
        glfwPollEvents(); // обрабатывает события
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate(); // освобождает ресурсы GLFW

    return 0;
}