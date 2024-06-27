#version 450

layout(location = 0) in vec2 inPosition;

layout(location = 0) out vec3 fragColor;

layout (binding = 0) buffer UniformBufferObject
{
    mat4 t[3];
} transforms;

layout (push_constant) uniform constants
{
    uint nbIteration;
    uint maxInstance;
    uint nbTransformation;
    uint padding;
    mat4 render_matrix;
} PushConstants;

void main() {
    gl_Position = PushConstants.render_matrix * vec4(inPosition, 0.0, 1.0);
    fragColor = vec3(0.5, (inPosition+1)/2);
}