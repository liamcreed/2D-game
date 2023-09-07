#pragma once


typedef struct
{
    sub_texture_t sub_texture;
    bool collision;
}tile_t;

#define MAX_TILEMAP_SIZE 128
#define MAX_TILE_COUNT 128
#define MAX_TILEMAP_LAYER_COUNT 2

typedef struct
{
    u32 width;
    u32 height;
    u32 tile_size;
    u32 layer_count;
    f32 layer_depths[MAX_TILEMAP_LAYER_COUNT];
    i32 indices[MAX_TILEMAP_LAYER_COUNT][MAX_TILEMAP_SIZE][MAX_TILEMAP_SIZE];
}tilemap_t;

typedef struct
{
    bool active;
    tile_t tiles[MAX_TILE_COUNT + 1];
    tilemap_t map;
    texture_t* texture;
}tilemap_component_t;
/* 
void tilemap_component_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer, entity_t e)
{
    tilemap_component_t* tile_c = &scene->tilemap_components[e];
    if (tile_c->active)
    {
        if (tile_c->map.width * tile_c->map.height > MAX_TILEMAP_SIZE * MAX_TILEMAP_SIZE)
            log_error("Maximim tilemap size exceeded");

        for (i32 l = 0; l < tile_c->map.layer_count; l++)
        {
            for (i32 y = 0; y < tile_c->map.height; y++)
            {
                for (i32 x = 0; x < tile_c->map.width; x++)
                {
                    u32 tile_id = tile_c->map.indices[l][tile_c->map.height - 1 - y][x];
                    if (tile_id != 0)
                    {
                        vec3 tile_location =
                        {
                            (x * tile_c->map.tile_size) - (tile_c->map.width * tile_c->map.tile_size) / 2.0 + trans_c->location.x + tile_c->map.tile_size / 2.0,
                            (y * tile_c->map.tile_size) - (tile_c->map.height * tile_c->map.tile_size) / 2.0 + trans_c->location.y + tile_c->map.tile_size / 2.0,
                            trans_c->location.z + tile_c->map.layer_depths[l]
                        };


                        bool tile_inside_frustum =
                            tile_location.x < scene->camera_components[scene->active_camera].frustum.max.x &&
                            tile_location.x > scene->camera_components[scene->active_camera].frustum.min.x &&
                            tile_location.y < scene->camera_components[scene->active_camera].frustum.max.y &&
                            tile_location.y > scene->camera_components[scene->active_camera].frustum.min.y;

                        if (tile_inside_frustum)
                        {
                            vec3 location = vec3_round(tile_location);

                            renderer_draw_sub_texture(renderer,
                                tile_c->texture,
                                &tile_c->tiles[tile_id].sub_texture,
                                location,
                                (vec2) {
                                tile_c->map.tile_size, tile_c->map.tile_size
                            },
                                (vec4) {
                                1.0, 1.0, 1.0, 1.0
                            });
                        }
                    }
                }
            }
        }
    }
} */