#pragma once

typedef struct 
{
    u32 fbo;
    u32 rbo;
    texture_t texture;
}framebuffer_t;

static f32 frame_buffer_vertices[] =
{
    -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0,  1.0f, 1.0f
};

void framebuffer_create(framebuffer_t* framebuffer);
void framebuffer_bind(framebuffer_t* framebuffer);
void framebuffer_unbind();
void framebuffer_delete(framebuffer_t* framebuffer);