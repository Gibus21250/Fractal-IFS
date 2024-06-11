#include <iostream>
#include <vector>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void IFS(std::vector<glm::vec2>& object, std::vector<glm::mat3>& transorms, uint32_t nbIteration);

int main()
{

    //Initialize small render engine
    Engine e;
    e.initEngine();

    //Triangle
    std::vector<glm::vec2> vertices = {
            {0.0f, -0.7f},
            {-0.5f, 0.5f},
            {0.5f, 0.5f}
    };


    std::vector<glm::mat3> transforms =
    {
        glm::mat3(0.5, 0, 0,
        0, 0.5, -0.25,
        0, 0, 1),

        glm::mat3(0.5, 0, -0.25,
                  0, 0.5, 0.25,
                  0, 0, 1),

        glm::mat3(0.5, 0, 0.25,
                  0, 0.5, 0.25,
                  0, 0, 1),

    };


    IFS(vertices, transforms, 1);


    e.createBuffer((void *) vertices.data(),  vertices.size() * sizeof(glm::vec2), vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


    e.run();
    e.destroy();

}

//Transform are 3D matrices bc of heterogenous coord
void IFS(std::vector<glm::vec2>& object, std::vector<glm::mat3>& transorms, uint32_t nbIteration)
{
    uint32_t padding = object.size();
    uint32_t nbMaxPoints = padding;

    for (int i = 0; i < nbIteration; ++i) {
        nbMaxPoints *= transorms.size();
    }

    object.resize(nbMaxPoints);

    //For each iteration
    for (int i = 0; i < nbIteration; ++i)
    {

        //For each transform, we write result from the end to the start of the buffer
        for (int j = 0; j < transorms.size(); ++j)
        {
            //We apply tranform for each point
            for (int k = 0; k < padding; ++k)
            {
                object[((transorms.size() - 1 + j) * padding) + k] = transorms[j] * glm::vec3(object[k], 0.0);
            }
        }
        //Update size of the total object
        padding = padding * transorms.size();
    }
}
