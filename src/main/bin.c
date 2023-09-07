#include "engine.h"
/* #include "main/player.h"
#include "main/enemy.h" */

int main()
{
    window_t window =
    {
        .width = 800,
        .height = 600,
        .resizable = false,
        .title = "hello",
        .cursor = true,
        .vsync = true
    };
    window_create(&window);

    renderer_t renderer =
    {
        .window = &window,
        .width = 480,
        .height = 480 / window.aspect,
        .clear_color = {34 / 255.0,34 / 255.0,34 / 255.0,1.0}
    };
    renderer_create(&renderer);

    texture_t texture_atlas = texture_load_from_TGA("./assets/textures/dev/texture_atlas.tga");

#define FRAME_RATE 12
    /* ecs_scene_t scene = {};
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
    scene.physics_components[player] = (physics_component_t)
    {
        .active = true,
        .aabb =
        {
            .min = {-1,-1},
            .max = {1,.2},
            .size = scene.sprite_components[player].size
        }
    };

    entity_t player_hitbox = ecs_create_entity(&scene, "player_hitbox");
    scene.data_components[player_hitbox].parent = player;
    scene.transform_components[player_hitbox].size = (vec2){ 16,16 };
    scene.transform_components[player_hitbox].location.x = 18;
    scene.transform_components[player_hitbox].location.y = -8;
    scene.physics_components[player_hitbox] = (physics_component_t)
    {
        .active = true,
        .aabb =
        {
            .min = {-1,-1},
            .max = {1,1},
            .size = {16,16},
        },
        .hit_collider = true,
    }; */
/* 
    scene.script_components[player].update = player_update;
    scene.script_components[player].context = &(player_script_context_t){ .hitbox = player_hitbox };
 */
    /* entity_t enemy = ecs_create_entity(&scene, "enemy");
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
        .anim_time = rand() % 4,
        .current_anim = 0
    };
    scene.transform_components[enemy] = (transform_component_t)
    {
        .size = scene.sprite_components[enemy].size,
        .location.x = rand() % 25 * 16 - 25 * 8,
        .location.y = rand() % 15 * 16 - 15 * 8,
        .location.z = 1.0,
    };

    scene.physics_components[enemy] = (physics_component_t)
    {
        .active = true,
        .fixed = false,
        .aabb =
        {
            .min = {-0.5,-1},
            .max = {0.5,0.8},
            .size = scene.sprite_components[enemy].size
        },
    }; */

/*     scene.script_components[enemy].update = enemy_update; */
/*     scene.script_components[enemy].context = &(enemy_script_context_t){ .player_hitbox = player_hitbox, .player = player}; */

   /*  texture_t background_texture = texture_load_from_TGA("./assets/textures/dev/map.tga");

    entity_t background = ecs_create_entity(&scene, "background");
    scene.sprite_components[background] = (sprite_component_t)
    {
        .active = true,
        .texture = &background_texture,
        .sub_texture = sub_texture(&background_texture, (vec2) {}, (vec2) { 16 * 30, 16 * 20 }, false),
        .color = {1,1,1,1},
        .size = {16 * 30,16 * 20}
    };
    scene.transform_components[background].size = scene.sprite_components[background].size; */

    while (!window.closed)
    {
        window_update(&window);
        renderer_start(&renderer);
        renderer.proj_mat = mat4_new(1);
        renderer.view_mat = mat4_new(1);
        renderer_draw_quad(&renderer, &texture_atlas, (vec3){}, (vec2){2,2}, (vec4){1,1,1,1});
        renderer_end(&renderer);
        /* ecs_update(&scene, &window, &renderer); */
    }

    texture_delete(&texture_atlas);
    /* ecs_exit(&scene); */
    renderer_exit(&renderer);
    window_exit(&window);
}

