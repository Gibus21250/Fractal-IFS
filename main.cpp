#include <iostream>

#include <GLFW/glfw3.h>

#include "utils/shader.hpp"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
GLFWwindow* window;


int main() {
    std::cout << "Hello, World!" << std::endl;

    if (!glfwInit())
    {
        std::cerr << "Error while init GLFW\n";
    }

    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        std::cerr << "Error while create Window\n";
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

