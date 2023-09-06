#include "engine.h"

void ecs_update_physics(ecs_scene_t* scene, window_t* window, renderer_t* renderer, bool debug);

void ecs_create_scene(ecs_scene_t* scene)
{
    scene->entity_count = 0;
    for (entity_t e = 0; e <= MAX_ENTITIES; e++)
    {
        ecs_reset_entity_values(scene, e);
    }
}

void ecs_reset_entity_values(ecs_scene_t* scene, entity_t e)
{
    scene->data_components[e] = (data_component_t)
    {
        .active = true,
        .unused = false,
        .parent = -1,
    };
    scene->transform_components[e] = (transform_component_t)
    {
        .size = {1.0f, 1.0f},
    };
    scene->camera_components[e] = (camera_component_t)
    {
        .near = -10,
        .far = 10,
        .ortho_size = 10,
    };
    scene->sprite_components[e] = (sprite_component_t)
    {
        .active = false,
        .color = {1.0, 1.0, 1.0, 1.0},
        .size = {1.0,1.0},
    };
    scene->physics_components[e] = (physics_component_t)
    {
        .aabb = {.min = {-1,-1}, .max = {1,1,}, .size = {1,1}}
    };
}

entity_t ecs_create_entity(ecs_scene_t* scene, char* name)
{
    entity_t e = 0;

    if (scene->entity_count != MAX_ENTITIES)
    {
        scene->entity_count += 1;
        e = scene->entity_count;
    }
    else
    {
        for (entity_t i = 1; i <= scene->entity_count; i++)
        {
            if (scene->data_components[i].unused)
                e = i;
        }

        if (e == 0)
        {
            log_error("[scene]: maximum amount of entities reached!\n");
        }
    }

    scene->data_components[e].unused = false;
    strcpy(scene->data_components[e].name, name);
    scene->data_components[e].name[strlen(scene->data_components[e].name) + 1] = '\0';
    return e;
}

void ecs_delete_entity(ecs_scene_t* scene, entity_t* entity)
{
    if (*entity != 0)
    {
        for (i32 e = 0; e < scene->entity_count - *entity; e++)
        {
            /* code */
        }
        ecs_reset_entity_values(scene, *entity);

        *entity = 0;
        return;
    }
}

void ecs_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer)
{
    for (entity_t e = 0; e <= scene->entity_count; e++)
    {
        if (scene->data_components[e].active && scene->script_components[e].update)
        {
            scene->script_components[e].update(scene, e, window, renderer, scene->script_components[e].context);
        }
    }

    renderer_start(renderer);


    for (entity_t e = 1; e <= scene->entity_count; e++)
    {
        if (scene->camera_components[e].active && scene->camera_components[e].active_camera)
        {
            scene->active_camera = e;
        }
    }

    {
        vec3 location = vec3_round(scene->transform_components[scene->active_camera].location);

        renderer->view_mat = mat4_translation(vec3_negate(location));
        renderer->proj_mat = mat4_ortho_aspect(window->aspect, scene->camera_components[scene->active_camera].ortho_size, scene->camera_components[scene->active_camera].near, scene->camera_components[scene->active_camera].far);

        vec2 frustum_size = { (scene->camera_components[scene->active_camera].ortho_size * window->aspect), (scene->camera_components[scene->active_camera].ortho_size) };
        scene->camera_components[scene->active_camera].frustum.min = (vec2){ -frustum_size.x + location.x, -frustum_size.y + location.y };
        scene->camera_components[scene->active_camera].frustum.max = (vec2){ frustum_size.x + location.x, frustum_size.y + location.y };
    }

    //-----------Physics-----------//
    ecs_update_physics(scene, window, renderer, false);

    for (entity_t e = 1; e <= scene->entity_count; e++)
    {
        if (scene->data_components[e].active)
        {
            transform_component_t* trans_c = &scene->transform_components[e];
            sprite_component_t* sprite_c = &scene->sprite_components[e];

            frustum_t frustum = scene->camera_components[scene->active_camera].frustum;

            bool inside_frustum =
                trans_c->location.x < frustum.max.x &&
                trans_c->location.x > frustum.min.x &&
                trans_c->location.y < frustum.max.y &&
                trans_c->location.y > frustum.min.y;


            if (scene->sprite_components[e].active)
            {
                if (sprite_c->animation_count != 0)
                {
                    sprite_animation_t* current_anim = &sprite_c->animations[sprite_c->current_anim];
                    sprite_c->anim_time += window->dt * current_anim->frame_rate;

                    if (sprite_c->anim_time >= current_anim->key_frame_count)
                    {
                        sprite_c->anim_time = 0.0;
                    }
                    sprite_c->sub_texture = sub_texture(sprite_c->texture, current_anim->key_frames[(u32)(sprite_c->anim_time)], sprite_c->size, sprite_c->flip_x);
                }

                if (inside_frustum)
                {
                    vec3 location = vec3_round(trans_c->location);

                    renderer_draw_sub_texture(renderer,
                        sprite_c->texture,
                        &sprite_c->sub_texture,
                        location,
                        trans_c->size,
                        sprite_c->color);
                }

            }
        }
    }
    renderer_end(renderer);
}

void ecs_exit(ecs_scene_t* scene)
{
    for (entity_t e = 0; e < MAX_ENTITIES + 1; e++)
    {
        ecs_reset_entity_values(scene, e);
    }
}