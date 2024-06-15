#include <iostream>
#include <vector>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void IFS(glm::vec2* object, size_t nb, std::vector<glm::mat3>& tranforms, uint32_t nbIteration);

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
