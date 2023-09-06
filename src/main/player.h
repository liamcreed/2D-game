#pragma once

typedef struct 
{
    entity_t hitbox;
}player_script_context_t;

void player_update(ecs_scene_t* scene, entity_t e, window_t* w, renderer_t* r, void* c)
{
    player_script_context_t* context = (player_script_context_t*)c;

    f32 speed = 96;
    if (!key_being_pressed(w, KEY_LEFT_SHIFT))
        speed = speed / 2.0;

    vec2 movement = {};
    if (key_being_pressed(w, KEY_D))
        movement.x = 1;
    if (key_being_pressed(w, KEY_A))
        movement.x = -1;
    if (key_being_pressed(w, KEY_W))
        movement.y = 1;
    if (key_being_pressed(w, KEY_S))
        movement.y = -1;

    if (movement.x < 0)
        scene->sprite_components[e].flip_x = true;
    if (movement.x > 0)
        scene->sprite_components[e].flip_x = false;


    if (movement.x > 0)
    {
        scene->transform_components[context->hitbox].location.x = 18;
    }
    if (movement.x < 0)
    {
        scene->transform_components[context->hitbox].location.x = -18;
    }

    if (movement.x || movement.y)
        scene->sprite_components[e].current_anim = 1;
    else
        scene->sprite_components[e].current_anim = 0;

    if (movement.x && movement.y)
    {
        movement.x *= cos(0.79);
        movement.y *= cos(0.79);
    }
    scene->physics_components[e].velocity = (vec2){ movement.x * speed, movement.y * speed };
}