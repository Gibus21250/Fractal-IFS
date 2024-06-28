#include <iostream>
#include <vector>
#include <chrono>

#include "engine/engine.h"
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

size_t g_iterations = 0;
Engine g_engine;

std::vector<glm::vec3 > g_points_init = {
        glm::vec3(0.0, -1.0, 0),
        glm::vec3(-1.0f, 1.0f, 0),
        glm::vec3(1.0f, 1.0f, 0)
};

//Triangle de Sierpiński
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

/*
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
*/
bool g_CPU = true;

void* g_uniformBuffer = nullptr;

void IFS(glm::vec3* object, size_t nb, std::vector<glm::mat4>& transforms, uint32_t nbIteration);
void computeIFS_CPU(std::vector<glm::vec3> init, std::vector<glm::mat4>& transforms, uint32_t iter);
void computeIFS_GPU(std::vector<glm::vec3>& init, std::vector<glm::mat4>& transforms, uint32_t iter);

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
    {

        g_iterations++;
        g_engine.clearDrawableObjects();
        g_uniformBuffer = nullptr;
        if(g_CPU)
            computeIFS_CPU(g_points_init, g_transforms, g_iterations);
        else
        {
            computeIFS_GPU(g_points_init, g_transforms, g_iterations);
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
                computeIFS_CPU(g_points_init, g_transforms, g_iterations);
            else
                computeIFS_GPU(g_points_init, g_transforms, g_iterations);
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
        g_uniformBuffer = nullptr;
        if(g_CPU)
            computeIFS_CPU(g_points_init, g_transforms, g_iterations);
        else
        {
            computeIFS_GPU(g_points_init, g_transforms, g_iterations);
        }
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        auto lookDir = glm::normalize(g_engine.getCamera().getLookAt() - g_engine.getCamera().getPosition());
        g_engine.getCamera().getPosition() += 0.1f * lookDir;
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        auto lookDir = glm::normalize(g_engine.getCamera().getLookAt() - g_engine.getCamera().getPosition());
        g_engine.getCamera().getPosition() -= 0.1f * lookDir;
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        auto lookDir = glm::normalize(g_engine.getCamera().getLookAt() - g_engine.getCamera().getPosition());
        auto right = glm::cross(lookDir, g_engine.getCamera().getUp());
        g_engine.getCamera().getPosition() += 0.1f * right;
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        auto lookDir = glm::normalize(g_engine.getCamera().getLookAt() - g_engine.getCamera().getPosition());
        auto right = glm::cross(lookDir, g_engine.getCamera().getUp());
        g_engine.getCamera().getPosition() -= 0.1f * right;
    }

}

int main()
{

    //Initialize small render engine
    g_engine.initEngine();
    glfwSetKeyCallback(g_engine.getGLFWindow(), keyboard_callback);
    computeIFS_CPU(g_points_init, g_transforms, g_iterations);

    g_engine.run();
    g_engine.destroy();
}

void computeIFS_CPU(std::vector<glm::vec3> init, std::vector<glm::mat4> &transforms, uint32_t iter)
{
    size_t nbMaxPoints = init.size();
    for (int i = 0; i < iter; ++i) {
        nbMaxPoints *= transforms.size();
    }

    std::cout << "Allocation needed: " << nbMaxPoints * sizeof(glm::vec3) << " bytes\n";
    if(nbMaxPoints * sizeof(glm::vec3) < (uint32_t)-1)
    {
        auto *buff = (glm::vec3 *) g_engine.createBuffer(nbMaxPoints * sizeof(glm::vec3),
                                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        //On copie nos points initiaux dans le buffer
        memcpy(buff, init.data(), sizeof(glm::vec3) * init.size());

        auto t1 = std::chrono::high_resolution_clock::now();
        IFS(buff, init.size(), transforms, iter);
        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << "Time spent: " << t2 - t1 << "\n";

        if (g_uniformBuffer == nullptr)
            g_uniformBuffer = g_engine.createBuffer(transforms.size() * sizeof(glm::mat4),
                                                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        //On met à jour l'uniformbuffer des transforms
        memcpy(g_uniformBuffer, transforms.data(), transforms.size() * sizeof(glm::mat4));

        std::vector<void *> res = {buff, g_uniformBuffer};

        std::vector<std::string> shad{"shaders/vert.spv", "shaders/frag.spv"};
        g_engine.addDrawableObject(shad, res, nbMaxPoints, 1);

        g_engine.pushConstants.maxInstance = 0;
    }
    else
    {
        std::cout << "Impossible to make this large allocation (4Go max)\nSwitch to GPU compute\n";
        g_CPU = false;
        computeIFS_GPU(init, transforms, iter);
    }
}

//Transform are 3D matrices bc of heterogeneous coord
void IFS(glm::vec3* object, size_t nbVertices, std::vector<glm::mat4>& trans, uint32_t nbIteration)
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
                object[((trans.size() - 1 - j)) * nbVertices + k] = glm::vec4(object[k], 1.0) * trans[j];
            }
        }
        //Update size of the total object
        nbVertices = nbVertices * trans.size();
    }
}

void computeIFS_GPU(std::vector<glm::vec3>& init, std::vector<glm::mat4>& transforms, uint32_t iter)
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
    auto* buff = (glm::vec3*) g_engine.createBuffer(init.size() * sizeof(glm::vec3), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    memcpy(buff, init.data(), init.size() * sizeof(glm::vec3));
    std::vector<void*> res = {buff, g_uniformBuffer};

    std::vector<std::string> shad{"shaders/instancedifsvert.spv", "shaders/frag.spv"};

    //On ajoute un objet drawable de l'élément de base, mais nbInstances fois
    g_engine.addDrawableObject(shad, res, 3, nbInstance);
}
