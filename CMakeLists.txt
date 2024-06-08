cmake_minimum_required(VERSION 3.20)
project(IFS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_INCLUDE_PATH "" CACHE PATH "third-party/glfw/include/GLFW/glfw.h")

add_subdirectory(third-party/glfw)
add_subdirectory(third-party/kompute)

#sur MCVS Kompute utilise un truc deprecié qui fait une erreur,
target_compile_definitions(kompute PRIVATE _CRT_SECURE_NO_WARNINGS=1)



include_directories(${kompute_SOURCE_DIR}/src/include ${GLFW_INCLUDE_PATH})


#target_include_directories(shader INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

include_directories(third-party/glm/glm third-party/glfw/include third-party/kompute/src/include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} glfw kompute::kompute)

add_subdirectory(shaders/)