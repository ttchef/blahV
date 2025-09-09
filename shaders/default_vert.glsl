
#version 450 core 

layout (binding = 0) uniform uniform_matrix_object {
    mat4 model;
} uniform_matrix;

layout (location = 0) in vec3 in_position;

void main() {
    gl_Position = uniform_matrix.model * vec4(in_position, 1.0);
}

