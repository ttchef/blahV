
#version 450 core 

layout (location = 0) in vec4 in_color;
layout (location = 1) in vec2 in_uv;

layout (location = 0) out vec4 out_color;

// Descriptor set 
layout (set = 0, binding = 0) uniform sampler2D in_sampled_texture;

void main() {
    vec4 tex_sample = texture(in_sampled_texture, in_uv);
    out_color = tex_sample;
}

