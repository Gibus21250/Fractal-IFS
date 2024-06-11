#include <iostream>
#include <vector>

#include <kompute/Kompute.hpp>

#include "engine/engine.h"
#include "engine/utils.h"

#include <glm/glm.hpp>

int main()
{

    //Initialize small render engine
    Engine e;
    e.initEngine();

    //Triangle
    const std::vector<Engine::Vertex> vertices = {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
    };

    e.createBuffer((void *) vertices.data(), 3 * sizeof(glm::vec2), 3, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


    e.run();

    e.destroy();

}
