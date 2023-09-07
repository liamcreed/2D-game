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


    entity_t wall = ecs_create_entity(&ecs, "wall");
    ecs.physics_components[wall] = (physics_component_t)
    {
        .active = true,
        .aabb = 
        {
            .min = {-1,-1},
            .max = {1,1},
            .size = {16,16}
        },
        .fixed = true,
    };

    ecs_scene_t scene1 = ecs_create_scene(&ecs, "scene0");
    ecs_scene_add_entity(&scene1, camera, "camera0");
    ecs_scene_add_entity(&scene1, player, "player");
    ecs_scene_add_entity(&scene1, player_hitbox, "player_hitbox");
    ecs_set_parent(&scene1, "player_hitbox", "player");

    for (i32 i = 0; i < 3; i++)
    {
        ecs_scene_add_entity(&scene1, enemy, "enemy");  
    }
    
    ecs_scene_add_entity(&scene1, background, "background");

    for (i32 i = 0; i < 5; i++)
    {
        char name[16];
        snprintf(name, 16, "wall %i\0", i);
        printf("%s\n", name);
        ecs_scene_add_entity(&scene1, wall, name);
        entity_t wall_entity = ecs_get_entity(&scene1, name);
      /*   
        ecs.transform_components[wall_entity].location.y = 8 * 16;
        ecs.transform_components[wall_entity].location.x = 8;
         */
    }
    
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

