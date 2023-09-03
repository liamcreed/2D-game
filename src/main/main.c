#include "engine.h"

int main()
{
    window_t window =
    {
        .width = 1080,
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

    texture_t texture_atlas = { .filter = false };
    texture_load_from_TGA(&texture_atlas, "./assets/textures/texture_atlas.tga");
    texture_create(&texture_atlas);

#include "scene.h"

    while (!window.closed)
    {
        window_update(&window);

        //-------------------------------------------------------------//

        f32 speed = 32;
        if (key_being_pressed(&window, KEY_LEFT_SHIFT))
            speed = 64;

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

        scene.transform_components[player].location.x += movement.x * speed * window.dt;
        scene.transform_components[player].location.y += movement.y * speed * window.dt;

        if (scene.transform_components[enemy].location.x > scene.transform_components[player].location.x)
            scene.sprite_components[enemy].flip_x = true;
        if (scene.transform_components[enemy].location.x < scene.transform_components[player].location.x)
            scene.sprite_components[enemy].flip_x = false;

        if (key_being_pressed(&window, KEY_SPACE))
            scene.transform_components[camera].location = vec3_lerp(scene.transform_components[camera].location, scene.transform_components[player].location, window.dt);
        //-------------------------------------------------------------//

        ecs_update(&scene, &window, &renderer);
    }

    texture_delete(&texture_atlas);
    ecs_exit(&scene);
    renderer_exit(&renderer);
    window_exit(&window);
}
