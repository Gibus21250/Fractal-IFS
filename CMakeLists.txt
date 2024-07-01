cmake_minimum_required(VERSION 3.21)
project(IFS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(third-party/glfw)

include_directories(${CMAKE_SOURCE_DIR}/third-party/glm/glm ${CMAKE_SOURCE_DIR}/third-party/glfw/include Vulkan::Headers)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        engine/engine.cpp
        engine/engine.h
        engine/utils.cpp
        engine/utils.h
        engine/camera.cpp
        engine/camera.h)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)

add_subdirectory(shaders/)