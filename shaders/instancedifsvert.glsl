#version 450

#extension GL_EXT_shader_explicit_arithmetic_types_float16 : require

layout (location = 0) in vec2 inPosition;
layout (location = 0) out vec3 fragColor;

layout (binding = 0) uniform UniformBufferObject
{
    mat4 t[3];  //maxUniformBufferRange: 65536
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
    float16_t currentValue = float16_t(gl_InstanceIndex) / float16_t(PushConstants.maxInstance - 1);
    float16_t start = currentValue;
    float16_t f_nbTransform = float16_t(PushConstants.nbTransformation);
    float16_t segmentSize = float16_t(1.0f) / f_nbTransform;

    mat4 model = mat4(1);

    for (int i = 0; i < PushConstants.nbIteration; i++)
    {
        //Get the indice of the transfom to apply
        float16_t f_no = floor(currentValue * f_nbTransform);
        uint no = min(uint(f_no), PushConstants.nbTransformation-1);
        model = model * transpose(transforms.t[no]);

        //Remap the value between 0 and 1 for the next iteration
        float16_t lower = float16_t(no) * segmentSize;
        currentValue = (currentValue - lower) / segmentSize;
    }

    gl_Position = model * vec4(inPosition, 0, 1.0);
    fragColor = vec3(1, float(start), 1.0);
}

//max(0, min(uint(no), PushConstants.nbTransformation-1))