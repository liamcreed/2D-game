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

    texture_t texture_atlas = texture_load_from_TGA("./assets/textures/dev/texture_atlas.tga");
    texture_t background_texture = texture_load_from_TGA("./assets/textures/dev/map.tga");

#define FRAME_RATE 12
    ecs_t ecs = {};
#include "scripts/scripts.h"
#include "entities.h"

    ecs_scene_t scene1 = ecs_create_scene(&ecs, "scene0");
    ecs_scene_add_entity(&scene1, camera, "camera0");
    ecs_scene_add_entity(&scene1, player, "player");
    ecs_scene_add_entity(&scene1, player_hitbox, "player_hitbox");
    ecs_set_parent(&scene1, "player_hitbox", "player");

    for (i32 i = 0; i < 100; i++)
    {
        ecs_scene_add_entity(&scene1, enemy, "enemy");  
    }
    
    ecs_scene_add_entity(&scene1, background, "background");

    while (!window.closed)
    {
        window_update(&window);
        renderer_start(&renderer);
        ecs_update(&scene1, &window, &renderer);
        renderer_end(&renderer);
    }

    texture_delete(&texture_atlas);

    renderer_exit(&renderer);
    window_exit(&window);
}

