#pragma once


entity_t camera = ecs_create_entity(&ecs, "camera");
ecs.camera_components[camera] = (camera_component_t)
{
    .active = true,
    .active_camera = true,
    .ortho_size = renderer.height,
    .near = -10,
    .far = 10,
};

//-----------------------------------//

entity_t player = ecs_create_entity(&ecs, "player");
ecs.sprite_components[player] = (sprite_component_t)
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
ecs.transform_components[player] = (transform_component_t)
{
    .size = {16,32},
    .location.z = 0.0,
};

ecs.physics_components[player] = (physics_component_t)
{
    .active = true,
    .fixed = false,
    .aabb =
    {
        .min = {-0.8,-1},
        .max = {0.8,.2},
        .size = {16,32},
    }
};

//-----------------------------------//

entity_t player_hitbox = ecs_create_entity(&ecs, "player_hitbox");
ecs.transform_components[player_hitbox].size = (vec2){ 16,16 };
ecs.transform_components[player_hitbox].location.x = 18;
ecs.transform_components[player_hitbox].location.y = -8;
ecs.physics_components[player_hitbox] = (physics_component_t)
{
    .active = true,
    .aabb =
    {
        .min = {-1,-1},
        .max = {1,1},
        .size = {16,16},
    },
    .hit_collider = true,
};

//-----------------------------------//

ecs.script_components[player] = (script_component_t)
{
    .active = true,
    .script = player_script,
};

//-----------------------------------//

entity_t enemy = ecs_create_entity(&ecs, "enemy");
ecs.sprite_components[enemy] = (sprite_component_t)
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
    .anim_time = rand() % 4,
    .current_anim = 0
};
ecs.transform_components[enemy] = (transform_component_t)
{
    .size = {32,32},
    .location.x = 50,
    .location.y = 50,
    .location.z = 1.0,
};

ecs.physics_components[enemy] = (physics_component_t)
{
    .active = true,
    .fixed = false,
    .aabb =
    {
        .min = {-0.5,-1},
        .max = {0.5,0.8},
        .size = {32,32},
    },
};
ecs.script_components[enemy] = (script_component_t)
{
    .active = true,
    .script = enemy_script,
};
