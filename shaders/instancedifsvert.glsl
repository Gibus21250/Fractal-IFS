#version 450

layout (location = 0) in vec2 inPosition;
layout (location = 0) out vec3 fragColor;

layout (binding = 0) uniform UniformBufferObject
{
    mat3 t[3];  //maxUniformBufferRange: 65536
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
    float value = float(gl_InstanceIndex) / float(PushConstants.maxInstance - 1);
    float segmentSize = 1.0f / float(PushConstants.nbTransformation);
    float f_nbTtransform = float(PushConstants.nbTransformation - 1);

    mat3 model = mat3(1);
    for (int i = 0; i < PushConstants.nbIteration; i++)
    {
        //Get the number of the transfom to apply
        float no = floor(value * f_nbTtransform);
        model = transforms.t[uint(no)] * model;

        //Remap the value between 0 and 1 for the next iteration
        float lower = no * segmentSize;
        value = (value - lower) / segmentSize;
    }

    gl_Position = PushConstants.render_matrix * vec4(model * vec3(inPosition, 1.0), 1.0);
    fragColor = vec3(0.5, gl_InstanceIndex / float(PushConstants.maxInstance), 1.0);
}

/*
*/

