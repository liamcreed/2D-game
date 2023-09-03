#pragma once

#define FRAME_RATE 12
ecs_scene_t scene = { .pixel_perfect = true };
ecs_create_scene(&scene);

entity_t camera = ecs_create_entity(&scene, "camera");
scene.camera_components[camera] = (camera_component_t)
{
    .active = true,
    .active_camera = true,
    .ortho_size = renderer.height,
    .near = -10,
    .far = 10,
};

entity_t player = ecs_create_entity(&scene, "player");
scene.sprite_components[player] = (sprite_component_t)
{
    .active = true,

    .size = {16,32},
    .texture = &texture_atlas,
    .color = {1.0,1.0,1.0,1.0},

    .animation_count = 2,
    .animations[0] =
    {
        .key_frame_count = 4,
        .frame_rate = FRAME_RATE,
        .key_frames[0] = {8,4},
        .key_frames[1] = {9,4},
        .key_frames[2] = {10,4},
        .key_frames[3] = {11,4}
    },
    .animations[1] =
    {
        .key_frame_count = 4,
        .frame_rate = FRAME_RATE,
        .key_frames[0] = {12,4},
        .key_frames[1] = {13,4},
        .key_frames[2] = {14,4},
        .key_frames[3] = {15,4}
    },
};
scene.transform_components[player].size = scene.sprite_components[player].size;
scene.transform_components[player].location.z = 0.0;

entity_t enemy = ecs_create_entity(&scene, "enemy");

scene.sprite_components[enemy].active = true;
scene.sprite_components[enemy] = (sprite_component_t)
{
    .active = true,
    .texture = &texture_atlas,
    .size = {32,32},

    .color = {1.0,1.0,1.0,1.0},

    .animation_count = 2,
    .animations[0] =
    {
        .key_frame_count = 4,
        .frame_rate = FRAME_RATE,
        .key_frames[0] = {0.5 + 0,13.5},
        .key_frames[1] = {0.5 + 1,13.5},
        .key_frames[2] = {0.5 + 2,13.5},
        .key_frames[3] = {0.5 + 3,13.5},
    },
    .animations[1] =
    {
        .key_frame_count = 4,
        .frame_rate = FRAME_RATE,
        .key_frames[0] = {0.5 + 4,13.5},
        .key_frames[1] = {0.5 + 5,13.5},
        .key_frames[2] = {0.5 + 6,13.5},
        .key_frames[3] = {0.5 + 7,13.5},
    },
    .anim_time = 0.6,
    .current_anim = 0
};
scene.transform_components[enemy].size = scene.sprite_components[enemy].size;
scene.transform_components[enemy].location.x = rand() % window.width - window.width / 2.0 - 50;
scene.transform_components[enemy].location.y = rand() % window.height - window.height / 2.0 - 50;
scene.transform_components[enemy].location.z = 1.0;

entity_t tile_map = ecs_create_entity(&scene, "tilemap");
scene.tilemap_components[tile_map] = (tilemap_component_t)
{
    .active = true,
    .map.tile_size = 16,
    .tiles[0] = {.sub_texture = sub_texture(&texture_atlas, (vec2) { 1,1 }, (vec2) { 16,16 }, false), .collision = false},
    .tiles[1] = {.sub_texture = sub_texture(&texture_atlas, (vec2) { 1,2 }, (vec2) { 16,16 }, false), .collision = false},

    .map.width = 10,
    .map.height = 10,
    .map.indices = 
    {
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,1,1,1,1,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0}
    },
    .texture = &texture_atlas
};