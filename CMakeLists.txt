cmake_minimum_required(VERSION 3.10)
project(TriangleOutput)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим библиотеки GLFW и Vulkan
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

# Добавляем исполняемый файл
add_executable(TriangleOutput triangleOut.cpp)

# Подключаем найденные библиотеки
target_link_libraries(TriangleOutput
    glfw
    GL
    Vulkan::Vulkan
    dl
    pthread
    X11
    Xrandr
    Xi
    Xxf86vm
)