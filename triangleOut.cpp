#include <GLFW/glfw3.h>


int main() {
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Color test", nullptr, nullptr);
    glfwMakeContextCurrent(window); // контекст — это объект, который хранит всё состояние графической машины

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window); // двойная буферизация для openGL-рендеринга. Без неё рисует в задний буфер бесконечно
        glfwPollEvents(); // обрабатывает события
    }

    glfwDestroyWindow(window);
    glfwTerminate(); // освобождает ресурсы GLFW

    return 0;
}