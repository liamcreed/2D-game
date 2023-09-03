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
        .location = {0.0f, 0.0f, 0.0f},
        .size = {1.0f, 1.0f},
    };
    scene->camera_components[e] = (camera_component_t)
    {
        .active_camera = false,
        .near = -10,
        .far = 10,
        .ortho_size = 10,
    };
    scene->sprite_components[e] = (sprite_component_t)
    {
        .active = false,
        .color = {1.0, 1.0, 1.0, 1.0},
        .size = {1.0,1.0},
        .texture = NULL
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

    renderer->view_mat = mat4_translation(vec3_negate(scene->transform_components[scene->active_camera].location));
    renderer->proj_mat = mat4_ortho_aspect(window->aspect, scene->camera_components[scene->active_camera].ortho_size, scene->camera_components[scene->active_camera].near, scene->camera_components[scene->active_camera].far);

    f32 frustum_bias = scene->camera_components[scene->active_camera].ortho_size / 3.0;
    vec2 frustum_size = { (scene->camera_components[scene->active_camera].ortho_size * window->aspect) / 2 + frustum_bias, (scene->camera_components[scene->active_camera].ortho_size) / 2 + frustum_bias };
    scene->camera_components[scene->active_camera].frustum.min = (vec2){ -frustum_size.x + scene->transform_components[scene->active_camera].location.x, -frustum_size.y + scene->transform_components[scene->active_camera].location.y };
    scene->camera_components[scene->active_camera].frustum.max = (vec2){ frustum_size.x + scene->transform_components[scene->active_camera].location.x, frustum_size.y + scene->transform_components[scene->active_camera].location.y };

    for (entity_t e = 1; e <= scene->entity_count; e++)
    {
        if (scene->data_components[e].active)
        {
            transform_component_t* trans_c = &scene->transform_components[e];

            if (scene->camera_components[e].active && scene->camera_components[e].active_camera)
            {
                scene->active_camera = e;
            }

            bool inside_frustum =
                trans_c->location.x < scene->camera_components[scene->active_camera].frustum.max.x &&
                trans_c->location.x > scene->camera_components[scene->active_camera].frustum.min.x &&
                trans_c->location.y < scene->camera_components[scene->active_camera].frustum.max.y &&
                trans_c->location.y > scene->camera_components[scene->active_camera].frustum.min.y;

            sprite_component_t* sprite_c = &scene->sprite_components[e];
            if (sprite_c->active)
            {
                if (sprite_c->animation_count != 0)
                {
                    animation_t* current_anim = &sprite_c->animations[sprite_c->current_anim];
                    sprite_c->anim_time += window->dt * current_anim->frame_rate;

                    if (sprite_c->anim_time >= current_anim->key_frame_count)
                    {
                        sprite_c->anim_time = 0.0;
                    }
                    sprite_c->sub_texture = sub_texture(sprite_c->texture, current_anim->key_frames[(u32)(sprite_c->anim_time)], sprite_c->size, sprite_c->flip_x);
                }

                if (inside_frustum)
                {
                    vec3 location;
                    if (scene->pixel_perfect)
                    {
                        location.x = round(trans_c->location.x);
                        location.y = round(trans_c->location.y);
                        location.z = round(trans_c->location.z);
                    }
                    else
                        location = trans_c->location;

                    renderer_draw_sub_texture(renderer,
                        sprite_c->texture,
                        &sprite_c->sub_texture,
                        location,
                        trans_c->size,
                        sprite_c->color);
                }
            }
            tilemap_component_t* tile_c = &scene->tilemap_components[e];
            if (tile_c->active && inside_frustum)
            {
                if (tile_c->map.width > MAX_TILEMAP_SIZE)
                    log_error("Maximim tilemap size exceeded");
                for (i32 y = 0; y < tile_c->map.height; y++)
                {
                    for (i32 x = 0; x < tile_c->map.width; x++)
                    {
                        u32 tile_id = tile_c->map.indices[tile_c->map.height - 1 - y ][x];
                        vec3 tile_location =
                        {
                            (x * tile_c->map.tile_size) - (tile_c->map.width * tile_c->map.tile_size) / 2.0 + trans_c->location.x + tile_c->map.tile_size / 2.0,
                            (y * tile_c->map.tile_size) - (tile_c->map.height * tile_c->map.tile_size) / 2.0 + trans_c->location.y + tile_c->map.tile_size / 2.0,
                            trans_c->location.z
                        };

                        bool tile_inside_frustum =
                            tile_location.x < scene->camera_components[scene->active_camera].frustum.max.x &&
                            tile_location.x > scene->camera_components[scene->active_camera].frustum.min.x &&
                            tile_location.y < scene->camera_components[scene->active_camera].frustum.max.y &&
                            tile_location.y > scene->camera_components[scene->active_camera].frustum.min.y;

                        if (tile_inside_frustum)
                        {
                            renderer_draw_sub_texture(renderer,
                                tile_c->texture,
                                &tile_c->tiles[tile_id].sub_texture,
                                tile_location,
                                (vec2) {
                                tile_c->map.tile_size, tile_c->map.tile_size
                            },
                                (vec4) {
                                1.0, 1.0, 1.0, 1.0
                            });
                        }
                    }
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