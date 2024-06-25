#version 450

layout (location = 0) in vec2 inPosition;
layout (location = 0) out vec3 fragColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 transforms[];  //maxUniformBufferRange: 65536
} ubo;

layout (push_constant) uniform constants
{
    uint32_t nbIteration;
    uint32_t maxInstance;
    uint32_t nbTransformation;
    uint32_t padding;
    mat4 render_matrix;
} PushConstants;

void main()
{
    float32_t value = float32_t(gl_InstanceIndex) / PushConstants.maxInstance - 1;
    float32_t segmentSize = 1.0f / PushConstants.nbTransformation;

    mat4 model = mat4(1);
    for (int i = 0; i < PushConstants.nbIteration; i++)
    {
        //Get the number of the transfom to apply
        uint32_t no = floor(value * PushConstants.nbTransformation);
        model = transform[no] * model;

        //Remap the value between 0 and 1 for the next iteration
        float32_t lower = no * segmentSize;
        value = (value - lower) / segmentSize;
    }

    gl_Position = PushConstants.render_matrix * model * vec4(inPosition, 0.0, 1.0);
}

