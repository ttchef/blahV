
# Default
glslc -fshader-stage=vert shaders/default_vert.glsl -o shaders/spv/default_vert.spv
glslc -fshader-stage=frag shaders/default_frag.glsl -o shaders/spv/default_frag.spv

# Texture
glslc -fshader-stage=vert shaders/texture_vert.glsl -o shaders/spv/texture_vert.spv
glslc -fshader-stage=frag shaders/texture_frag.glsl -o shaders/spv/texture_frag.spv

