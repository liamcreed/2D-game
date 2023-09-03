#include "graphics/graphics.h"


void renderer_create(renderer_t* renderer)
{
    renderer->vertices = malloc(MAX_QUAD_COUNT * 4 * sizeof(vertex_t));

    renderer->indices = malloc(MAX_QUAD_COUNT * 6 * sizeof(u32));

    i32 index_offset = 0;
    for (u32 i = 0; i < MAX_QUAD_COUNT * 6; i += 6)
    {
        renderer->indices[i + 0] = index_offset + 0;
        renderer->indices[i + 1] = index_offset + 1;
        renderer->indices[i + 2] = index_offset + 2;
        renderer->indices[i + 3] = index_offset + 1;
        renderer->indices[i + 4] = index_offset + 2;
        renderer->indices[i + 5] = index_offset + 3;
        index_offset += 4;
    }

    GL(glEnable(GL_BLEND));
    GL(glEnable(GL_DEPTH_TEST));

    GL(glGenVertexArrays(1, &renderer->quad_vertex_array.vao));
    GL(glBindVertexArray(renderer->quad_vertex_array.vao));
    if (renderer->quad_vertex_array.vao == 0)
        log_error("Failed to create vertex array");

    GL(glGenBuffers(1, &renderer->quad_vertex_array.vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vertex_array.vbo));
    if (renderer->quad_vertex_array.vbo == 0)
        log_error("Failed to create vertex buffer");
    GL(glBufferData(GL_ARRAY_BUFFER, MAX_QUAD_COUNT * 4 * sizeof(vertex_t), NULL, GL_DYNAMIC_DRAW));

    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, location)));

    GL(glEnableVertexAttribArray(1));
    GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, uv)));

    GL(glEnableVertexAttribArray(2));
    GL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, color)));

    GL(glEnableVertexAttribArray(3));
    GL(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, tex_index)));

    GL(glGenBuffers(1, &renderer->quad_vertex_array.ibo));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->quad_vertex_array.ibo));
    if (renderer->quad_vertex_array.ibo == 0)
        log_error("Failed to create vertex buffer");
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_QUAD_COUNT * 6 * sizeof(u32), renderer->indices, GL_STATIC_DRAW));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    renderer->empty_texture = (texture_t)
    {
        .channel_count = 4,
        .width = 1,
        .height = 1,
        .data = malloc(4)
    };
    memset(renderer->empty_texture.data, 255, 4);
    texture_create(&renderer->empty_texture);

    renderer->texture_count = 1;
    renderer->textures[0] = &renderer->empty_texture;

    renderer->quad_shader = (shader_t)
    {
        .v_path = "./assets/shaders/quad.vert",
        .f_path = "./assets/shaders/quad.frag"
    };
    shader_create(&renderer->quad_shader);

    i32 samplers[MAX_TEXTURE_COUNT];
    for (i32 i = 0; i < MAX_TEXTURE_COUNT; i++) samplers[i] = i;

    shader_set_uniform_int_arr(&renderer->quad_shader, "u_textures", samplers, 16);

    renderer->post_process_shader = (shader_t)
    {
        .v_path = "./assets/shaders/pp.vert",
        .f_path = "./assets/shaders/pp.frag",
    };
    shader_create(&renderer->post_process_shader);
    renderer->scene_buffer.texture = (texture_t)
    {
        .width = renderer->width,
        .height = renderer->height,
    };
    framebuffer_create(&renderer->scene_buffer);

    GL(glGenVertexArrays(1, &renderer->frame_buffer_va.vao));
    GL(glBindVertexArray(renderer->frame_buffer_va.vao));
    if (renderer->frame_buffer_va.vao == 0)
        log_error("Failed to create vertex array");

    GL(glGenBuffers(1, &renderer->frame_buffer_va.vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, renderer->frame_buffer_va.vbo));
    if (renderer->frame_buffer_va.vbo == 0)
        log_error("Failed to create vertex buffer");
    GL(glBufferData(GL_ARRAY_BUFFER, sizeof(frame_buffer_vertices), frame_buffer_vertices, GL_DYNAMIC_DRAW));

    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (const void*)0));

    GL(glEnableVertexAttribArray(1));
    GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (const void*)(3 * sizeof(f32))));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void renderer_exit(renderer_t* renderer)
{
    GL(glDeleteVertexArrays(1, &renderer->quad_vertex_array.vao));
    GL(glDeleteBuffers(1, &renderer->quad_vertex_array.vbo));
    GL(glDeleteBuffers(1, &renderer->quad_vertex_array.ibo));

    free(renderer->vertices);
    free(renderer->indices);
}

void renderer_start(renderer_t* renderer)
{
    shader_bind(&renderer->quad_shader);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_proj", renderer->proj_mat);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_view", renderer->view_mat);


    framebuffer_bind(&renderer->scene_buffer);
    GL(glEnable(GL_DEPTH_TEST));

    GL(glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

}

void renderer_flush(renderer_t* renderer)
{
    size_t vertices_size = renderer->vertex_count * sizeof(vertex_t);
    GL(glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vertex_array.vbo));
    GL(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, renderer->vertices));

    for (i32 i = 0; i < renderer->texture_count; i++)
    {
        texture_bind(renderer->textures[i], i);
    }

    shader_bind(&renderer->quad_shader);

    GL(glBindVertexArray(renderer->quad_vertex_array.vao));
    GL(glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0));

    renderer->index_count = 0;
    renderer->texture_count = 1;
    renderer->vertex_count = 0;
}

void renderer_end(renderer_t* renderer)
{
    renderer_flush(renderer);

    framebuffer_unbind();
    GL(glDisable(GL_DEPTH_TEST));

#ifdef __APPLE__
    GL(glViewport(0, 0, renderer->window->width * 2, renderer->window->height * 2));
#elif _WIN32
    GL(glViewport(0, 0, renderer->window->width, renderer->window->height));
#endif

    GL(glClearColor(1.0, 1.0, 1.0, 1.0));
    GL(glClear(GL_COLOR_BUFFER_BIT));

    shader_bind(&renderer->post_process_shader);
    texture_bind(&renderer->scene_buffer.texture, 0);

    GL(glBindVertexArray(renderer->frame_buffer_va.vao));
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void renderer_draw_quad(renderer_t* renderer, texture_t* texture, vec3 location, vec2 size, vec4 color)
{
    if (renderer->vertex_count > MAX_QUAD_COUNT * 4 - 1 || renderer->texture_count > MAX_TEXTURE_COUNT - 1)
        renderer_flush(renderer);

    u32 tex_index = 0;
    if (texture != NULL)
    {
        for (i32 i = 0; i < renderer->texture_count; i++)
        {
            if (texture == renderer->textures[i])
            {
                tex_index = i;
                break;
            }
        }

        if (tex_index == 0)
        {
            renderer->texture_count++;
            tex_index = renderer->texture_count - 1;
            renderer->textures[renderer->texture_count - 1] = texture;
        }
    }

    vertex_t* vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x - (size.x / 2.0), location.y - (size.y / 2.0), location.z };
    vertex->uv = (vec2){ 0.0, 0.0 };
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x + (size.x / 2.0), location.y - (size.y / 2.0), location.z };
    vertex->uv = (vec2){ 1.0, 0.0 };
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x - (size.x / 2.0), location.y + (size.y / 2.0), location.z };
    vertex->uv = (vec2){ 0.0, 1.0 };
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x + (size.x / 2.0), location.y + (size.y / 2.0), location.z };
    vertex->uv = (vec2){ 1.0, 1.0 };
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    renderer->index_count += 6;
}

void renderer_draw_sub_texture(renderer_t* renderer, texture_t* texture, sub_texture_t* sub_texture, vec3 location, vec2 size, vec4 color)
{
    if (renderer->vertex_count > MAX_QUAD_COUNT * 4 - 1 || renderer->texture_count > MAX_TEXTURE_COUNT - 1)
        renderer_flush(renderer);

    u32 tex_index = 0;

    if (texture)
    {
        for (i32 i = 0; i < renderer->texture_count; i++)
        {
            if (texture == renderer->textures[i]) //maybe compare whole struct
            {
                tex_index = i;
                break;
            }
        }

        if (tex_index == 0)
        {
            renderer->texture_count++;
            tex_index = renderer->texture_count - 1;
            renderer->textures[renderer->texture_count - 1] = texture;
        }
    }

    vertex_t* vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x - (size.x / 2.0), location.y - (size.y / 2.0), location.z };
    vertex->uv = sub_texture->uv[0];
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x + (size.x / 2.0), location.y - (size.y / 2.0), location.z };
    vertex->uv = sub_texture->uv[1];
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x - (size.x / 2.0), location.y + (size.y / 2.0), location.z };
    vertex->uv = sub_texture->uv[2];
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    vertex = &renderer->vertices[renderer->vertex_count];
    vertex->location = (vec3){ location.x + (size.x / 2.0), location.y + (size.y / 2.0), location.z };
    vertex->uv = sub_texture->uv[3];
    vertex->color = color;
    vertex->tex_index = tex_index;
    renderer->vertex_count++;

    renderer->index_count += 6;
}
