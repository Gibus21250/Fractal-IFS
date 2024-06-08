#version 450

layout(push_constant) uniform PushConstants {
    float x;
    float y;
    float z;
} pcs;

layout (local_size_x = 1) in;

layout(set = 0, binding = 0) buffer a { float pa[]; };

void main() {
    pa[0] = pa[0] + pcs.x;
    pa[1] = pa[1] + pcs.y;
    pa[2] = pa[1] + pcs.z;
}