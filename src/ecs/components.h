#pragma once

typedef struct
{
    bool active;
    bool unused;
    char name[32];
    char type[32];
} data_component_t;

typedef struct
{
    vec3 location;
    vec2 size;
} transform_component_t;

typedef struct
{
    vec2 min, max;
}frustum_t;

typedef struct
{
    bool active;
    bool active_camera;

    f32 ortho_size, near, far;

    frustum_t frustum;
}camera_component_t;

#define MAX_KEY_FRAME_COUNT 16
typedef struct
{
    u32 frame_rate;
    u32 key_frame_count;
    vec2 key_frames[MAX_KEY_FRAME_COUNT];
}sprite_animation_t;

#define MAX_ANIMATION_COUNT 16
typedef struct
{
    bool active;

    texture_t* texture;
    sub_texture_t sub_texture;

    vec2 size;
    bool flip_x;

    vec4 color;

    u32 animation_count;
    sprite_animation_t animations[MAX_ANIMATION_COUNT];

    u32 current_anim;
    f64 anim_time;
}sprite_component_t;

typedef struct
{
    vec2 min,max;
    vec2 size;
    vec2 center;
}aabb_t;

typedef struct
{
    bool active;
    bool fixed;
    vec2 velocity;

    vec2 size;
    aabb_t aabb;

    bool collided_with[MAX_ENTITIES+1];
    bool collided;
}physics_component_t;

#include "ecs/tilemap.h"