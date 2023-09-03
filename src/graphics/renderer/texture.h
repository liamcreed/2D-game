#pragma once

typedef struct
{
    u32 id;
    bool filter, sRGB;

    u32 width;
    u32 height;
    u32 channel_count;
    u8* data;
} texture_t;

typedef struct
{
    vec2 coords;
    vec2 size;
    texture_t* tilemap;
    bool flip_x;
    bool flip_y;
}sprite_t;

typedef struct
{
    vec2 uv[4];
}sub_texture_t;

void texture_load_from_TGA(texture_t *texture, const char *path);

void texture_export_to_bin(texture_t* texture, const char* path);
void texture_TGA_to_bin(const char* TGA, const char* bin);
void texture_load_from_bin(texture_t *texture, const char *path);
void texture_create(texture_t *texture);
void texture_bind(texture_t *texture, u32 index);
void texture_unbind(u32 index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture);

sub_texture_t sub_texture(texture_t* texture, vec2 coords, vec2 size, bool flip_x);