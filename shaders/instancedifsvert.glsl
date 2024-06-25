#version 450

layout (location = 0) in vec2 inPosition;
layout (location = 0) out vec3 fragColor;

layout (binding = 0) buffer UniformBufferObject
{
    mat4 t[];  //maxUniformBufferRange: 65536
} transforms;

layout (push_constant) uniform constants
{
    uint nbIteration;
    uint maxInstance;
    uint nbTransformation;
    uint padding;
    mat4 render_matrix;
} PushConstants;

void main()
{
    float value = float(gl_InstanceIndex) / PushConstants.maxInstance - 1;
    float segmentSize = 1.0f / PushConstants.nbTransformation;

    mat4 model = mat4(1);
    for (int i = 0; i < PushConstants.nbIteration; i++)
    {
        //Get the number of the transfom to apply
        uint no = uint(floor(value * PushConstants.nbTransformation));
        model = transforms.t[no] * model;

        //Remap the value between 0 and 1 for the next iteration
        float lower = no * segmentSize;
        value = (value - lower) / segmentSize;
    }

    gl_Position = PushConstants.render_matrix * model * vec4(inPosition, 0.0, 1.0);
}

