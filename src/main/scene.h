#pragma once


/* 
entity_t tile_map = ecs_create_entity(&scene, "tilemap");
scene.tilemap_components[tile_map] = (tilemap_component_t)
{
    .active = true,
    .map.tile_size = 16,
    .tiles[0] = {},
    .tiles[1] = {.visible = true, .sub_texture = sub_texture(&texture_atlas, (vec2) { 1,1 }, (vec2) { 16,16 }, false), .collision = false},
    .tiles[2] = {.visible = true, .sub_texture = sub_texture(&texture_atlas, (vec2) { 1,2 }, (vec2) { 16,16 }, false), .collision = false},
    .tiles[3] = {.visible = true, .sub_texture = sub_texture(&texture_atlas, (vec2) { 23,4 }, (vec2) { 16,16 }, false), .collision = false},

    .map.width = 10,
    .map.height = 10,
    .map.layer_count = 2,
    .map.layer_depths[0] = 0,
    .map.layer_depths[1] = 5,
    .map.indices[0] =
    {
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
    },
    .map.indices[1] =
    {
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,3,0,0,0,0,3,0,0,0},
        {0,0,0,3,0,3,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    },
    .texture = &texture_atlas
}; */

