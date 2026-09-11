#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>

#include <vector>


int main() {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); //принуждение GLFW к X11

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Color test", nullptr, nullptr);
    // glfwMakeContextCurrent(window); // контекст — это объект, который хранит всё состояние графической машины

    uint32_t glfwExtentionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount); // какие инстанс-расширения нужны, чтобы работать с окнами
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

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window); // двойная буферизация для openGL-рендеринга. Без неё рисует в задний буфер бесконечно
        glfwPollEvents(); // обрабатывает события
    }

    glfwDestroyWindow(window);
    glfwTerminate(); // освобождает ресурсы GLFW

    return 0;
}