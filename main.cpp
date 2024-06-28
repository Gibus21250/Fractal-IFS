#include <iostream>
#include <vector>
#include <chrono>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

size_t g_iterations = 0;
Engine g_engine;

std::vector<glm::vec2> g_init = {
        glm::vec2(0.0, -1.0),
        glm::vec2(-1.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
};


std::vector<glm::mat4> g_transforms =
        {
                glm::mat4(0.5, 0, 0, 0,
                          0, 0.5, 0, -0.5,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0, 0,-0.5,
                          0, 0.5, 0,0.5,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0, 0, 0.5,
                          0, 0.5, 0, 0.5,
                          0, 0, 1, 0,
                          0, 0, 0, 1)

        };
*/

/*
//Arbre de pythagore
std::vector<glm::mat4> g_transforms =
        {
                glm::mat4(0.5, 0, 0, 0,
                          0, 0.5, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, -0.5, 0,0.5,
                          0.5, 0.5, 0,0,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0.5, 0, 0,
                          -0.5, 0.5, 0, 0.5,
                          0, 0, 1, 0,
                          0, 0, 0, 1)

        };
*/

//Le flocon de Koch
/*
std::vector<glm::mat4> g_transforms =
        {
                glm::mat4(1.0/3, 0, 0, 0,
                          0, 1.0/3, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(1.0/6, -sqrt(3)/6, 0,1.0/3,
                          sqrt(3)/6, 1.0/6, 0,0,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(1.0/6, sqrt(3)/6, 0, 1.0/2,
                          -sqrt(3)/6, 1.0/6, 0, sqrt(3)/6,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

                glm::mat4(1.0/3, 0, 0, 2.0/3,
                          0, 1.0/3, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1),

        };
*/

//Tetrahèdre de Sierpiǹski

std::vector<glm::mat4> g_transforms =
        {
                glm::mat4(0.5, 0, 0, 0,
                          0, 0.5, 0, 0,
                          0, 0, 0.5, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0, 0,0.5,
                          0, 0.5, 0,0,
                          0, 0, 0.5, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0, 0, 0.25,
                          0, 0.5, 0, sqrt(3)/4,
                          0, 0, 0.5, 0,
                          0, 0, 0, 1),

                glm::mat4(0.5, 0, 0, 0.25,
                          0, 0.5, 0, sqrt(3)/12,
                          0, 0, 0.5, sqrt(6)/6,
                          0, 0, 0, 1),

        };

bool g_CPU = true;

void* g_uniformBuffer = nullptr;

void IFS(glm::vec2* object, size_t nb, std::vector<glm::mat4>& transforms, uint32_t nbIteration);
void computeIFS_CPU(std::vector<glm::vec2>& init, std::vector<glm::mat4>& transforms, uint32_t iter);
void computeIFS_GPU(std::vector<glm::vec2>& init, std::vector<glm::mat4>& transforms, uint32_t iter);

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
    {

        g_iterations++;
        g_engine.clearDrawableObjects();
        if(g_CPU)
            computeIFS_CPU(g_init, g_transforms, g_iterations);
        else
        {
            g_uniformBuffer = nullptr;
            computeIFS_GPU(g_init, g_transforms, g_iterations);
        }

    }
    else if(key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
    {
        if(g_iterations)
        {
            g_iterations--;
            g_engine.clearDrawableObjects();
            g_uniformBuffer = nullptr;
            if(g_CPU)
                computeIFS_CPU(g_init, g_transforms, g_iterations);
            else
                computeIFS_GPU(g_init, g_transforms, g_iterations);
        }
    }
    else if(key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        g_engine.switchConservativeRaster();
    }
    else if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        g_CPU = !g_CPU;
        g_engine.clearDrawableObjects();
        if(g_CPU)
            computeIFS_CPU(g_init, g_transforms, g_iterations);
        else
        {
            g_uniformBuffer = nullptr;
            computeIFS_GPU(g_init, g_transforms, g_iterations);
        }

    }
}

int main()
{

    //Initialize small render engine
    g_engine.initEngine();
    glfwSetKeyCallback(g_engine.getGLFWindow(), keyboard_callback);
    computeIFS_GPU(g_init, g_transforms, g_iterations);

    g_engine.run();
    g_engine.destroy();
}

void computeIFS_CPU(std::vector<glm::vec2>& init, std::vector<glm::mat4> &transforms, uint32_t iter)
{
    size_t nbMaxPoints = init.size();
    for (int i = 0; i < iter; ++i) {
        nbMaxPoints *= transforms.size();
    }

    std::cout << "Allocation needed: " << nbMaxPoints * sizeof(glm::vec2) << " bytes\n";
    auto* buff = (glm::vec2*) g_engine.createBuffer(nbMaxPoints * sizeof(glm::vec2), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    memcpy(buff, init.data(), sizeof(glm::vec2) * init.size());

    auto t1 = std::chrono::high_resolution_clock::now();
    IFS(buff, 3, transforms, iter);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time spent: " << t2 - t1 << "\n";

    std::vector<void*> res = {buff};


    g_engine.addDrawableObject(res, nbMaxPoints);
}

//Transform are 3D matrices bc of heterogeneous coord
void IFS(glm::vec2* object, size_t nbVertices, std::vector<glm::mat4>& trans, uint32_t nbIteration)
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
                object[((trans.size() - 1 - j)) * nbVertices + k] = glm::vec4(object[k], 1.0, 1.0) * trans[j];
            }
        }
        //Update size of the total object
        nbVertices = nbVertices * trans.size();
    }
}

void computeIFS_GPU(std::vector<glm::vec2>& init, std::vector<glm::mat4>& transforms, uint32_t iter)
{
    //Si l'uniform buffer n'a pas été initialisé
    if(g_uniformBuffer == nullptr)
        g_uniformBuffer = g_engine.createBuffer(transforms.size() * sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //On met à jour l'uniformbuffer des transforms
    memcpy(g_uniformBuffer, transforms.data(), transforms.size() * sizeof(glm::mat4));

    //On met à jour le pushconstant
    g_engine.pushConstants.nbTransformation = transforms.size();
    g_engine.pushConstants.nbIteration = iter;
    size_t nbInstance = 1;
    for (int i = 0; i < iter; ++i) {
        nbInstance *= transforms.size();
    }
    g_engine.pushConstants.maxInstance = nbInstance;

    //On crée un vertexBuffer de l'élément initial
    auto* buff = (glm::vec2*) g_engine.createBuffer(init.size() * sizeof(glm::vec2), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    memcpy(buff, init.data(), init.size() * sizeof(glm::vec2));
    std::vector<void*> res = {buff, g_uniformBuffer};

    //On ajoute un objet drawable de l'élément de base, mais nbInstances fois
    g_engine.addDrawableObject(res, 3, nbInstance);
}
