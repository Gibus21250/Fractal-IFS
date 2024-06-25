#include <iostream>
#include <vector>
#include <chrono>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

size_t iterations = 0;
Engine e;

std::vector<glm::vec2> init = {
        glm::vec2(0.0, -1.0),
        glm::vec2(-1.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
};


std::vector<glm::mat3> transforms =
        {
                glm::mat3(0.5, 0, 0,
                          0, 0.5, -0.5,
                          0, 0, 1),

                glm::mat3(0.5, 0, -0.5,
                          0, 0.5, 0.5,
                          0, 0, 1),

                glm::mat3(0.5, 0, 0.5,
                          0, 0.5, 0.5,
                          0, 0, 1),

        };

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
        {
            iterations--;
            e.clearDrawableObjects();
            drawIFS(init, transforms, iterations);
        }
    }
    else if(key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        e.switchConservativeRaster();
    }
}

int main()
{

    //Initialize small render engine
    e.initEngine();
    glfwSetKeyCallback(e.getGLFWindow(), keyboard_callback);

    drawIFS(init, transforms, iterations);

    e.run();
    e.destroy();
}

void drawIFS(std::vector<glm::vec2>& ini, std::vector<glm::mat3>& trans, uint32_t iter)
{
    size_t nbMaxPoints = ini.size();
    for (int i = 0; i < iter; ++i) {
        nbMaxPoints *= trans.size();
    }

    std::cout << "Allocation needed: " << nbMaxPoints * sizeof(glm::vec2) << " bytes\n";
    auto* buff = (glm::vec2*) e.createBuffer(nbMaxPoints * sizeof(glm::vec2), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    memcpy(buff, ini.data(), sizeof(glm::vec2) * ini.size());

    auto t1 = std::chrono::high_resolution_clock::now();
    IFS(buff, 3, trans, iter);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time spent: " << t2 - t1 << "\n";

    std::vector<void*> res = {buff};

    e.addDrawableObject(res, nbMaxPoints);
}

//Transform are 3D matrices bc of heterogeneous coord
void IFS(glm::vec2* object, size_t nbVertices, std::vector<glm::mat3>& trans, uint32_t nbIteration)
{
    //For each iteration
    for (int i = 0; i < nbIteration; ++i)
    {

        //For each transform, we write result from the end to the start of the buffer
        for (size_t j = 0; j < trans.size(); ++j)
        {
            //We apply tranform for each point
            for (size_t k = 0; k < nbVertices; ++k)
            {
                object[((trans.size() - 1 - j)) * nbVertices + k] = glm::vec3(object[k], 1.0) * trans[j];
            }
        }
        //Update size of the total object
        nbVertices = nbVertices * trans.size();
    }
}
