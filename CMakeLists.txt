cmake_minimum_required(VERSION 3.28)
project(IFS)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_PATH "" CACHE PATH "third-party/include/GLFW/glfw.h" )
set(GLFW_LIBRARY "" CACHE FILEPATH "third-party/libs/libglfw3.a" )

set(GLM_INCLUDE_PATH "" CACHE PATH "third-party/include/glm/glm.hpp" )


link_directories(/third-party/libs)
add_executable(IFS main.cpp)
target_link_libraries(IFS glfw)

