#include "engine.h"

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

    texture_t texture_atlas = { .filter = false };
    texture_load_from_TGA(&texture_atlas, "./assets/textures/dev/texture_atlas.tga");
    texture_create(&texture_atlas);

#define FRAME_RATE 12
    ecs_scene_t scene = {};
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

    entity_t enemies[20];
    for (i32 i = 0; i < 20; i++)
    {
        enemies[i] = ecs_create_entity(&scene, "enemy");

        scene.sprite_components[enemies[i]].active = true;
        scene.sprite_components[enemies[i]] = (sprite_component_t)
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
        scene.transform_components[enemies[i]].size = scene.sprite_components[enemies[i]].size;
        scene.transform_components[enemies[i]].location.x = rand() % 25*16 -  25*8;
        scene.transform_components[enemies[i]].location.y = rand() % 15*16 -  15*8;
        scene.transform_components[enemies[i]].location.z = 1.0;
        scene.physics_components[enemies[i]] = (physics_component_t)
        {
            .active = true,
            /* .fixed = true, */
            .aabb =
            {
                .min = {-0.5,-1},
                .max = {0.5,0.8},
                .size = scene.sprite_components[enemies[i]].size
            },
        };
    }



    texture_t background_texture = { .filter = false };
    texture_load_from_TGA(&background_texture, "./assets/textures/dev/map.tga");
    texture_create(&background_texture);

    entity_t background = ecs_create_entity(&scene, "background");
    scene.sprite_components[background] = (sprite_component_t)
    {
        .active = true,
        .texture = &background_texture,
        .sub_texture = sub_texture(&background_texture, (vec2) {}, (vec2) { 16 * 30, 16 * 20 }, false),
        .color = {1,1,1,1},
        .size = {16 * 30,16 * 20}
    };
    scene.transform_components[background].size = scene.sprite_components[background].size;

#include "scene.h"

    while (!window.closed)
    {
        window_update(&window);

        //-------------------------------------------------------------//
        f32 speed = 96;
        if (!key_being_pressed(&window, KEY_LEFT_SHIFT))
            speed = speed / 2.0;

        vec2 movement = {};
        if (key_being_pressed(&window, KEY_D))
            movement.x = 1;
        if (key_being_pressed(&window, KEY_A))
            movement.x = -1;
        if (key_being_pressed(&window, KEY_W))
            movement.y = 1;
        if (key_being_pressed(&window, KEY_S))
            movement.y = -1;

        if (movement.x < 0)
            scene.sprite_components[player].flip_x = true;
        if (movement.x > 0)
            scene.sprite_components[player].flip_x = false;

        if (movement.x || movement.y)
            scene.sprite_components[player].current_anim = 1;
        else
            scene.sprite_components[player].current_anim = 0;

        if (movement.x && movement.y)
        {
            movement.x *= cos(0.79);
            movement.y *= cos(0.79);
        }
        scene.physics_components[player].velocity = (vec2){ movement.x * speed, movement.y * speed};

/*         if (scene.transform_components[enemy].location.x > scene.transform_components[player].location.x)
            scene.sprite_components[enemy].flip_x = true;
        if (scene.transform_components[enemy].location.x < scene.transform_components[player].location.x)
            scene.sprite_components[enemy].flip_x = false;
 */
        if (key_being_pressed(&window, KEY_SPACE))
            printf("%f\n", 1.0/window.dt);

        //-------------------------------------------------------------//

        ecs_update(&scene, &window, &renderer);
    }

    texture_delete(&texture_atlas);
    ecs_exit(&scene);
    renderer_exit(&renderer);
    window_exit(&window);
}

