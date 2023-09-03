#version 330 core

in vec3  v_pos;
in vec2  v_uv;
in vec4  v_color;
in float   v_texture;

out vec4 f_color;

uniform sampler2D u_textures[16];

void main()
{
    vec4 color = texture(u_textures[int(v_texture)], v_uv) * v_color;
    if(color.a < 0.1)
        discard;
    f_color = color;
}