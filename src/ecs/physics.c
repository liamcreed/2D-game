#include "engine.h"

void ecs_update_physics(ecs_scene_t* scene, window_t* window, renderer_t* renderer, bool debug)
{
    if (debug)
        renderer_flush(renderer);

    bool collision_checked_entities[MAX_ENTITY_COUNT + 1][MAX_ENTITY_COUNT + 1] = { {false},{false} };
    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->data_components[e].active)
        {
            transform_component_t* trans_c = &scene->transform_components[e];
            transform_component_t trans_c_parent;
            entity_t parent = scene->data_components[e].parent;
            if (parent != -1)
            {
                trans_c_parent = (transform_component_t)
                {
                    .location = vec3_add(scene->transform_components[e].location, scene->transform_components[parent].location),
                    .size = scene->transform_components[e].size
                };
                trans_c = &trans_c_parent;
            }

            sprite_component_t* sprite_c = &scene->sprite_components[e];
            physics_component_t* phys_c = &scene->physics_components[e];

            frustum_t frustum = scene->camera_components[scene->active_camera].frustum;

            bool inside_frustum =
                trans_c->location.x < frustum.max.x &&
                trans_c->location.x > frustum.min.x &&
                trans_c->location.y < frustum.max.y &&
                trans_c->location.y > frustum.min.y;

            if (phys_c->active && inside_frustum)
            {
                for (entity_t f = 0; f < scene->entity_count; f++)
                {
                    physics_component_t* phys_c_f = &scene->physics_components[f];
                    if (f != e && phys_c_f->active && !collision_checked_entities[e][f] && !collision_checked_entities[f][e] && scene->data_components[f].active)
                    {
                        transform_component_t* trans_c_f = &scene->transform_components[f];
                        transform_component_t trans_c_parent_f;
                        entity_t parent_f = scene->data_components[f].parent;

                        if (parent_f != -1)
                        {
                            trans_c_parent_f = (transform_component_t)
                            {
                                .location = vec3_add(scene->transform_components[parent_f].location, trans_c_f->location),
                                .size = trans_c_f->size
                            };
                            trans_c_f = &trans_c_parent_f;
                        }


                        frustum_t collision_frustum = scene->camera_components[scene->active_camera].frustum;
                        bool collision_inside_frustum =
                            trans_c_f->location.x < collision_frustum.max.x / 10.0 + trans_c_f->location.x &&
                            trans_c_f->location.x > collision_frustum.min.x / 10.0 + trans_c_f->location.x &&
                            trans_c_f->location.y < collision_frustum.max.y / 10.0 + trans_c_f->location.y &&
                            trans_c_f->location.y > collision_frustum.min.y / 10.0 + trans_c_f->location.y;

                        if (collision_inside_frustum)
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
                                .min.x = trans_c_f->location.x + (phys_c_f->aabb.size.x / 2) * phys_c_f->aabb.min.x,
                                .min.y = trans_c_f->location.y + (phys_c_f->aabb.size.y / 2) * phys_c_f->aabb.min.y,
                                .max.x = trans_c_f->location.x + (phys_c_f->aabb.size.x / 2) * phys_c_f->aabb.max.x,
                                .max.y = trans_c_f->location.y + (phys_c_f->aabb.size.y / 2) * phys_c_f->aabb.max.y,
                            };

                            if (!(e_aabb.max.x < f_aabb.min.x
                                || e_aabb.min.x > f_aabb.max.x
                                || e_aabb.max.y < f_aabb.min.y
                                || e_aabb.min.y > f_aabb.max.y))
                            {
                                if (debug)
                                {
                                    renderer_draw_aabb(renderer, e_aabb.min, e_aabb.max, (vec4) { 1, 0, 0, 1 });
                                    renderer_draw_aabb(renderer, f_aabb.min, f_aabb.max, (vec4) { 1, 0, 0, 1 });
                                }

                                phys_c->collided = true;
                                phys_c->collided_with[f] = true;
                                phys_c_f->collided = true;
                                phys_c_f->collided_with[e] = true;

                                // collision response
                                if (!phys_c->hit_collider)
                                {
                                    vec2 relative_velocity = vec2_subtract
                                    (
                                        phys_c->velocity,
                                        phys_c_f->velocity
                                    );
                                    e_aabb.center = (vec2){ trans_c->location.x,  trans_c->location.y };
                                    f_aabb.center = (vec2){ trans_c_f->location.x,  trans_c_f->location.y };

                                    vec2 collision_normal = vec2_normalize(vec2_subtract(e_aabb.center, f_aabb.center));
                                    f32 normal_velocity = vec2_dot(relative_velocity, collision_normal);

                                    if (normal_velocity <= 0)
                                    {
                                        //sphys_c_f->velocity = phys_c->velocity;
                                        vec2 impulse = vec2_multiply_f32(collision_normal, -normal_velocity);
                                        phys_c_f->velocity = phys_c->velocity;
                                        phys_c->velocity = impulse;

                                    }
                                }
                            }
                            else
                            {
                                phys_c->collided = false;
                                phys_c->collided_with[f] = false;
                                phys_c_f->collided = false;
                                phys_c_f->collided_with[e] = false;
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
        }
    }

    if (debug)
    {
        renderer->wireframe = true;
        renderer_flush(renderer);
        renderer->wireframe = false;
    }

}