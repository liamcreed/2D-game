#version 330 core
layout(location = 0) in vec3  a_pos;
layout(location = 1) in vec2  a_uv;
layout(location = 2) in vec4  a_color;
layout(location = 3) in float   a_texture;

uniform mat4 u_view;
uniform mat4 u_proj;

out vec3  v_pos;
out vec2  v_uv;
out vec4  v_color;
out float v_texture;

void main()
{
    gl_Position = u_proj * u_view * vec4(a_pos, 1.0);

    v_pos = a_pos;
    v_uv = a_uv;
    v_color = a_color;
    v_texture = a_texture;
}