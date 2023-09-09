#include "engine.h"

int main()
{
    window_t window =
    {
        .width = 1280,
        .height = 720,
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

#define GRID_SIZE 480

    texture_t grid_texture =
    {
        .channel_count = 4,
        .data = malloc(GRID_SIZE * GRID_SIZE * 4),
        .width = GRID_SIZE,
        .height = GRID_SIZE,
    };
    memset(grid_texture.data, 255, GRID_SIZE * GRID_SIZE * 4);

    texture_create(&grid_texture);

    entity_t background = ecs_create_entity(&ecs, "background");
    ecs.sprite_components[background] = (sprite_component_t)
    {
        .active = true,
        .texture = &grid_texture,
        .color = {1,1,1,1},
    };
    ecs.transform_components[background] = (transform_component_t)
    {
       .size = {480, 480}
    };

    ecs_scene_t scene1 = ecs_create_scene(&ecs, "scene0");
    ecs_scene_add_entity(&scene1, camera, "camera0");
    ecs_scene_add_entity(&scene1, player, "player");
    ecs_scene_add_entity(&scene1, player_hitbox, "player_hitbox");
    ecs_set_parent(&scene1, "player_hitbox", "player");

    ecs_scene_add_entity(&scene1, enemy, "enemy");

    ecs_scene_add_entity(&scene1, background, "background");

    typedef struct
    {
        vec4 color;
        i32 id;
    }particle_t;
    particle_t (*particles)[GRID_SIZE] = malloc(GRID_SIZE * GRID_SIZE * sizeof(particle_t));

    while (!window.closed)
    {
        window_update(&window);

        for (i32 y = 0; y < GRID_SIZE; y++)
        {
            for (i32 x = 0; x < GRID_SIZE; x++)
            {
                particles[x][y].color = (vec4){(f32)rand()/RAND_MAX,0,0,1};

                i32 c = (x * 4) + (y * GRID_SIZE * 4);
                grid_texture.data[c + 0] = particles[x][y].color.x *  255;
                grid_texture.data[c + 1] = particles[x][y].color.y * 255;
                grid_texture.data[c + 2] = particles[x][y].color.z * 255;
                grid_texture.data[c + 3] = 255;
            }
        }
        texture_update_data(&grid_texture);

        renderer_start(&renderer);
        ecs_update(&scene1, &window, &renderer);
        renderer_end(&renderer);
    }

    texture_delete(&texture_atlas);

    renderer_exit(&renderer);
    window_exit(&window);
}

