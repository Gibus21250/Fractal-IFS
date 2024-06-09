cmake_minimum_required(VERSION 3.21)
project(IFS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(third-party/glfw)
add_subdirectory(third-party/kompute)

#sur MCVS Kompute utilise un truc deprecié qui fait une erreur,
target_compile_definitions(kompute PRIVATE _CRT_SECURE_NO_WARNINGS=1)

include_directories(third-party/glm/glm third-party/glfw/include third-party/kompute/src/include Vulkan::Headers)

find_package(Vulkan REQUIRED)



add_executable(${PROJECT_NAME} main.cpp
        engine/engine.cpp
        engine/engine.h
        engine/utils.cpp
        engine/utils.h)
target_link_libraries(${PROJECT_NAME} glfw kompute::kompute Vulkan::Vulkan)

add_subdirectory(shaders/)