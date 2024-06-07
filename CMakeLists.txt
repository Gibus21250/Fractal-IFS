cmake_minimum_required(VERSION 3.20)
project(IFS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(third-party/glfw)
add_subdirectory(third-party/kompute)

target_compile_definitions(kompute PRIVATE _CRT_SECURE_NO_WARNINGS=1)

set(GLFW_INCLUDE_PATH "" CACHE PATH "third-party/glfw/include/GLFW/glfw.h")

include_directories(${kompute_SOURCE_DIR}/src/include)


# Compiling shader
# To add more shaders simply copy the vulkan_compile_shader command and replace it with your new shader
vulkan_compile_shader(
    INFILE shader/my_shader.comp
    OUTFILE shader/my_shader.hpp
    NAMESPACE "shader")

# Then add it to the library, so you can access it later in your code
#add_library(shader INTERFACE "shader/my_shader.hpp")
#target_include_directories(shader INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

include_directories(third-party/glm/glm third-party/glfw/include third-party/kompute/src/include)

add_executable(${PROJECT_NAME} main.cpp utils/shader.cpp)
target_link_libraries(${PROJECT_NAME} glfw kompute::kompute)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)