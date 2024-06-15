#version 450

layout(location = 0) in vec2 inPosition;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = vec3(0.5, (inPosition+1)/2);
}