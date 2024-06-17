#include <iostream>
#include <vector>
#include <chrono>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void IFS(glm::vec2* object, size_t nb, std::vector<glm::mat3>& transforms, uint32_t nbIteration);
void drawIFS(std::vector<glm::vec2>& init, std::vector<glm::mat3>& transforms, uint32_t iter);

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
    {
        iterations++;
        e.clearDrawableObjects();
        drawIFS(init, transforms, iterations);
    }
    else if(key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
    {
        if(iterations)
            iterations--;
        e.clearDrawableObjects();
        drawIFS(init, transforms, iterations);
    }
}

int main()
{

    //Initialize small render engine
    Engine e;
    e.initEngine();
    glfwSetKeyCallback(e.getGLFWindow(), keyboard_callback);

    size_t nbIteration = 7, nbMaxPoints = 3;

    for (int i = 0; i < nbIteration; ++i) {
        nbMaxPoints *= transforms.size();
    }

    std::cout << "Allocation needed: " << nbMaxPoints * sizeof(glm::vec2) << " bytes\n";
    auto* buff = (glm::vec2*) e.createBuffer(nbMaxPoints * sizeof(glm::vec2), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    buff[0] = glm::vec2(0.0f, -1.0f);
    buff[1] = glm::vec2(-1.0f, 1.0f);
    buff[2] = glm::vec2(1.0f, 1.0f);

    auto t1 = std::chrono::high_resolution_clock::now();
    IFS(buff, 3, transforms, nbIteration);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time spent: " << t2 - t1 << "\n";

    std::vector<void*> res = {buff};

    e.addDrawableObject(res, nbMaxPoints);

    e.run();
    e.destroy();
}

//Transform are 3D matrices bc of heterogeneous coord
void IFS(glm::vec2* object, size_t nbVertices, std::vector<glm::mat3>& tranforms, uint32_t nbIteration)
{
    //For each iteration
    for (int i = 0; i < nbIteration; ++i)
    {

        //For each transform, we write result from the end to the start of the buffer
        for (size_t j = 0; j < tranforms.size(); ++j)
        {
            //We apply tranform for each point
            for (size_t k = 0; k < nbVertices; ++k)
            {
                object[((tranforms.size() - 1 - j)) * nbVertices + k] = glm::vec3(object[k], 1.0) * tranforms[j];
            }
        }
        //Update size of the total object
        nbVertices = nbVertices * tranforms.size();
    }
}
