#include "engine.h"

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
        .unused = true,
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
    if (entity != 0)
    {
        ecs_reset_entity_values(scene, *entity);
        *entity = 0;
        scene->entity_count -= 1;
    }
}

void ecs_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer)
{
    renderer_start(renderer);

    {
        vec3 location = vec3_round(scene->transform_components[scene->active_camera].location);

        renderer->view_mat = mat4_translation(vec3_negate(location));
        renderer->proj_mat = mat4_ortho_aspect(window->aspect, scene->camera_components[scene->active_camera].ortho_size, scene->camera_components[scene->active_camera].near, scene->camera_components[scene->active_camera].far);

        f32 frustum_bias = scene->camera_components[scene->active_camera].ortho_size / 3.0;
        vec2 frustum_size = { (scene->camera_components[scene->active_camera].ortho_size * window->aspect) / 2 + frustum_bias, (scene->camera_components[scene->active_camera].ortho_size) / 2 + frustum_bias };
        scene->camera_components[scene->active_camera].frustum.min = (vec2){ -frustum_size.x + location.x, -frustum_size.y + location.y };
        scene->camera_components[scene->active_camera].frustum.max = (vec2){ frustum_size.x + location.x, frustum_size.y + location.y };
    }

    bool collision_checked_entities[MAX_ENTITIES + 1][MAX_ENTITIES + 1] = { {false},{false} };

    for (entity_t e = 1; e <= scene->entity_count; e++)
    {
        if (scene->data_components[e].active)
        {
            transform_component_t* trans_c = &scene->transform_components[e];
            sprite_component_t* sprite_c = &scene->sprite_components[e];
            physics_component_t* phys_c = &scene->physics_components[e];

            bool inside_frustum =
                trans_c->location.x < scene->camera_components[scene->active_camera].frustum.max.x &&
                trans_c->location.x > scene->camera_components[scene->active_camera].frustum.min.x &&
                trans_c->location.y < scene->camera_components[scene->active_camera].frustum.max.y &&
                trans_c->location.y > scene->camera_components[scene->active_camera].frustum.min.y;

            if (phys_c->active && inside_frustum)
            {
                for (entity_t f = 1; f <= scene->entity_count; f++)
                {
                    if (!collision_checked_entities[e][f] && !collision_checked_entities[f][e])
                    {
                        frustum_t collision_frustum = scene->camera_components[scene->active_camera].frustum;
                        bool collision_inside_frustum =
                            scene->transform_components[f].location.x < collision_frustum.max.x / 10.0 + scene->transform_components[f].location.x &&
                            scene->transform_components[f].location.x > collision_frustum.min.x / 10.0 + scene->transform_components[f].location.x &&
                            scene->transform_components[f].location.y < collision_frustum.max.y / 10.0 + scene->transform_components[f].location.y &&
                            scene->transform_components[f].location.y > collision_frustum.min.y / 10.0 + scene->transform_components[f].location.y;

                        if (f != e && scene->physics_components[f].active && collision_inside_frustum)
                        {
                            aabb_t e_aabb =
                            {
                                .min.x = trans_c->location.x + (phys_c->aabb.size.x / 2) * phys_c->aabb.min.x,
                                .min.y = trans_c->location.y + (phys_c->aabb.size.y / 2) * phys_c->aabb.min.y,
                                .max.x = trans_c->location.x + (phys_c->aabb.size.x / 2) * phys_c->aabb.max.x,
                                .max.y = trans_c->location.y + (phys_c->aabb.size.y / 2) * phys_c->aabb.max.y,
                            };

                            aabb_t f_aabb =
                            {
                                .min.x = scene->transform_components[f].location.x + (scene->physics_components[f].aabb.size.x / 2) * scene->physics_components[f].aabb.min.x,
                                .min.y = scene->transform_components[f].location.y + (scene->physics_components[f].aabb.size.y / 2) * scene->physics_components[f].aabb.min.y,
                                .max.x = scene->transform_components[f].location.x + (scene->physics_components[f].aabb.size.x / 2) * scene->physics_components[f].aabb.max.x,
                                .max.y = scene->transform_components[f].location.y + (scene->physics_components[f].aabb.size.y / 2) * scene->physics_components[f].aabb.max.y,
                            };

                            if (!(e_aabb.max.x < f_aabb.min.x
                                || e_aabb.min.x > f_aabb.max.x
                                || e_aabb.max.y < f_aabb.min.y
                                || e_aabb.min.y > f_aabb.max.y))
                            {
                                /*                                 renderer_draw_aabb(renderer, e_aabb.min, e_aabb.max, (vec4) { 1, 0, 0, 1 });
                                                                renderer_draw_aabb(renderer, f_aabb.min, f_aabb.max, (vec4) { 1, 0, 0, 1 });
                                 */
                                scene->physics_components[e].collided = true;
                                scene->physics_components[e].collided_with[f] = true;
                                scene->physics_components[f].collided = true;
                                scene->physics_components[f].collided_with[e] = true;

                                // collision response
                                if (!phys_c->fixed)
                                {
                                    vec2 relative_velocity = vec2_subtract
                                    (
                                        scene->physics_components[e].velocity,
                                        scene->physics_components[f].velocity
                                    );
                                    e_aabb.center = (vec2){ scene->transform_components[e].location.x,  scene->transform_components[e].location.y };
                                    f_aabb.center = (vec2){ scene->transform_components[f].location.x,  scene->transform_components[f].location.y };

                                    vec2 collision_normal = vec2_normalize(vec2_subtract(e_aabb.center, f_aabb.center));
                                    f32 normal_velocity = vec2_dot(relative_velocity, collision_normal);

                                    if (normal_velocity <= 0)
                                    {
                                        if (scene->physics_components[f].fixed)
                                        {
                                            collision_normal = vec2_round(collision_normal);
                                            if (phys_c->velocity.x > 0 && collision_normal.x < 0)
                                                phys_c->velocity.x = 0;
                                            else if (phys_c->velocity.x < 0 && collision_normal.x > 0)
                                                phys_c->velocity.x = 0;
                                            else if (phys_c->velocity.y > 0 && collision_normal.y < 0)
                                                phys_c->velocity.y = 0;
                                            else if (phys_c->velocity.y < 0 && collision_normal.y > 0)
                                                phys_c->velocity.y = 0;
                                        }
                                        else
                                        {
                                            vec2 impulse = vec2_multiply_f32(collision_normal, -normal_velocity);
                                            scene->physics_components[f].velocity = phys_c->velocity;
                                            scene->physics_components[e].velocity = impulse;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                scene->physics_components[e].collided = false;
                                scene->physics_components[e].collided_with[f] = false;
                                scene->physics_components[f].collided = false;
                                scene->physics_components[f].collided_with[e] = false;
                            }

                            collision_checked_entities[e][f] = true;
                            collision_checked_entities[f][e] = true;
                        }
                    }
                }
                if (!phys_c->fixed)
                {
                    vec3 velocity = { phys_c->velocity.x, phys_c->velocity.y, 0.0 };
                    trans_c->location = vec3_add(trans_c->location, vec3_multiply_f32(velocity, window->dt));
                }
            }

            if (sprite_c->active)
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

            if (scene->camera_components[e].active && scene->camera_components[e].active_camera)
            {
                scene->active_camera = e;
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