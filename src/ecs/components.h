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
    vec2 min;
    vec2 max;
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
}animation_t;

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
    animation_t animations[MAX_ANIMATION_COUNT];

    u32 current_anim;
    f64 anim_time;
}sprite_component_t;

typedef struct
{
    bool visible;
    sub_texture_t sub_texture;
    bool collision;
}tile_t;

#define MAX_TILEMAP_SIZE 128
#define MAX_TILE_COUNT 64
#define MAX_TILEMAP_LAYER_COUNT 2

typedef struct
{
    u32 width;
    u32 height;
    u32 tile_size;
    u32 layer_count;
    f32 layer_depths[MAX_TILEMAP_LAYER_COUNT];
    u32 indices[MAX_TILEMAP_LAYER_COUNT][MAX_TILEMAP_SIZE][MAX_TILEMAP_SIZE];
}tilemap_t;

typedef struct
{
    bool active;
    tile_t tiles[MAX_TILE_COUNT];
    tilemap_t map;
    texture_t* texture;
}tilemap_component_t;

