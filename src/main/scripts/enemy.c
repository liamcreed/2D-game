#include "engine.h"

void enemy_init(ecs_scene_t* scene, entity_t e)
{
    scene->transform_components[e].location.x = rand() % 128;
    scene->transform_components[e].location.y = rand() % 128;
    scene->transform_components[e].location.z = 5;
}
void enemy_update(ecs_scene_t* scene, entity_t e, window_t* w, renderer_t* r, void* c)
{
    entity_t player = ecs_get_entity(scene, "player");
    entity_t player_hitbox = ecs_get_entity(scene, "player_hitbox");

    if (scene->transform_components[e].location.x > scene->transform_components[player].location.x)
        scene->sprite_components[e].flip_x = true;
    if (scene->transform_components[e].location.x < scene->transform_components[player].location.x)
        scene->sprite_components[e].flip_x = false;

    vec2 player_direction = vec2_subtract((vec2) { scene->transform_components[player].location.x, scene->transform_components[player].location.y }, (vec2) { scene->transform_components[e].location.x, scene->transform_components[e].location.y });

    if (vec2_length(player_direction) < 128)
    {
        scene->sprite_components[e].current_anim = 1;
        scene->physics_components[e].velocity = vec2_multiply_f32(vec2_normalize(player_direction), 16);
    }
    else
    {
        scene->sprite_components[e].current_anim = 0;
        scene->physics_components[e].velocity = (vec2){ 0,0 };
    }

    if (scene->physics_components[e].collided_with[player_hitbox] && key_being_pressed(w, KEY_J))
        scene->data_components[e].active = false;
}

ecs_script_t enemy_script = { .update = enemy_update, .init = enemy_init};