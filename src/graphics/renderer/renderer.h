#pragma once

typedef struct
{
    vec3 location;
    vec2 uv;
    vec4 color;
    f32 tex_index;
}vertex_t;

typedef struct
{
    u32 vao;
    u32 vbo;
    u32 ibo;
}vertex_array_t;


#define MAX_QUAD_COUNT 1000
#define MAX_TEXTURE_COUNT 16
typedef struct
{
    window_t* window;
    u32 width;
    u32 height;
    vec4 clear_color;

    vertex_t* vertices;
    u32 vertex_count;

    u32* indices;
    u32 index_count;

    texture_t* textures[MAX_TEXTURE_COUNT]; //TODO: use pointers instead
    u32 texture_count;
    texture_t empty_texture;

    vertex_array_t quad_vertex_array;
    shader_t quad_shader;

    mat4 view_mat;
    mat4 proj_mat;

    framebuffer_t scene_buffer;
    vertex_array_t frame_buffer_va;
    shader_t post_process_shader;

    bool wireframe;
}renderer_t;

void renderer_create(renderer_t* renderer);
void renderer_exit(renderer_t* renderer);

void renderer_start(renderer_t* renderer);
void renderer_flush(renderer_t* renderer);
void renderer_end(renderer_t* renderer);

void renderer_draw_quad(renderer_t* renderer, texture_t* texture, vec3 location, vec2 size, vec4 color);
void renderer_draw_sub_texture(renderer_t* renderer, texture_t* texture, sub_texture_t* sub_texture, vec3 location, vec2 size, vec4 color);
void renderer_draw_aabb(renderer_t* renderer, vec2 min, vec2 max, vec4 color);