
#version 450 core 

layout (binding = 0) uniform uniform_matrix_object {
    mat4 model;
} uniform_matrix;

layout (push_constant) uniform push_constant {
    mat4 model;
    vec4 color;
} pc;

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec4 out_color;

void main() {
    gl_Position = pc.model * vec4(in_position, 1.0);
    out_color = pc.color;
}

