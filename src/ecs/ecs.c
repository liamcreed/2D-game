#include "engine.h"

void ecs_update_physics(ecs_scene_t* scene, window_t* window, renderer_t* renderer, bool debug);

entity_t ecs_create_entity(ecs_t* ecs, char* name)
{
    entity_t e = -1;

    if (ecs->entity_count < MAX_ENTITY_COUNT)
    {
        e = ecs->entity_count;
        ecs->entity_count += 1;
    }
    else
    {
        for (entity_t i = 0; i < ecs->entity_count; i++)
        {
            if (ecs->data_components[i].unused)
                e = i;
        }

        if (e == -1)
        {
            log_error("[scene]: maximum amount of entities reached!\n");
        }
    }

    if (e > -1)
    {
        ecs->data_components[e].active = true;
        ecs->data_components[e].unused = false;
        ecs->data_components[e].parent = -1;
        strcpy(ecs->data_components[e].name, name);
        ecs->data_components[e].name[strlen(ecs->data_components[e].name) + 1] = '\0';
    }
    return e;
}

ecs_scene_t ecs_create_scene(ecs_t* ecs, char* name)
{
    ecs_scene_t scene;

    for (entity_t e = 0; e <= MAX_ENTITY_COUNT; e++)
    {
        ecs_reset_entity_values(&scene, e);
    }

    scene.ecs = ecs;

    strcpy(scene.name, name);
    scene.name[strlen(scene.name) + 1] = '\0';
    scene.entity_count = 0;

    return scene;
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
    scene->script_components[e] = (script_component_t)
    {

    };
}

void ecs_scene_add_entity(ecs_scene_t* scene, entity_t e, char* name)
{
    if (scene->entity_count < MAX_ENTITY_COUNT)
    {
        scene->data_components[scene->entity_count] = scene->ecs->data_components[e];

        strcpy(scene->data_components[scene->entity_count].name, name);
        scene->data_components[scene->entity_count].name[strlen(scene->data_components[scene->entity_count].name) + 1] = '\0';

        scene->transform_components[scene->entity_count] = scene->ecs->transform_components[e];
        scene->camera_components[scene->entity_count] = scene->ecs->camera_components[e];
        scene->sprite_components[scene->entity_count] = scene->ecs->sprite_components[e];
        scene->script_components[scene->entity_count] = scene->ecs->script_components[e];
        scene->physics_components[scene->entity_count] = scene->ecs->physics_components[e];

        if (scene->script_components[scene->entity_count].active && scene->script_components[scene->entity_count].script.init)
            scene->script_components[scene->entity_count].script.init(scene, scene->entity_count);
        
        scene->entity_count++;
    }
}
void ecs_delete_entity(ecs_scene_t* scene, entity_t entity)
{
    ecs_reset_entity_values(scene, entity);
}

void ecs_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer)
{
    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->data_components[e].active && scene->camera_components[e].active && scene->camera_components[e].active_camera)
        {
            scene->active_camera = e;
            vec3 location = vec3_round(scene->transform_components[scene->active_camera].location);

            renderer->view_mat = mat4_translation(vec3_negate(location));
            renderer->proj_mat = mat4_ortho_aspect(window->aspect, scene->camera_components[scene->active_camera].ortho_size, scene->camera_components[scene->active_camera].near, scene->camera_components[scene->active_camera].far);

            vec2 frustum_size = { (scene->camera_components[scene->active_camera].ortho_size * window->aspect), (scene->camera_components[scene->active_camera].ortho_size) };
            scene->camera_components[scene->active_camera].frustum.min = (vec2){ -frustum_size.x + location.x, -frustum_size.y + location.y };
            scene->camera_components[scene->active_camera].frustum.max = (vec2){ frustum_size.x + location.x, frustum_size.y + location.y };
        }
    }

    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->data_components[e].active && scene->script_components[e].active && scene->script_components[e].script.update)
        {
            scene->script_components[e].script.update(scene, e, window, renderer, scene->script_components[e].script.context);
        }
    }

    ecs_update_physics(scene, window, renderer, true);

    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (scene->data_components[e].active)
        {
            transform_component_t* trans_c = &scene->transform_components[e];
            sprite_component_t* sprite_c = &scene->sprite_components[e];

            if (sprite_c->active)
            {
                frustum_t frustum = scene->camera_components[scene->active_camera].frustum;

                bool inside_frustum =
                    trans_c->location.x < frustum.max.x &&
                    trans_c->location.x > frustum.min.x &&
                    trans_c->location.y < frustum.max.y &&
                    trans_c->location.y > frustum.min.y;


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

                    bool has_subtexture =
                        sprite_c->sub_texture.uv[0].x != 0 &&
                        sprite_c->sub_texture.uv[1].x != 0 &&
                        sprite_c->sub_texture.uv[2].x != 0 &&
                        sprite_c->sub_texture.uv[3].x != 0 &&
                        sprite_c->sub_texture.uv[0].y != 0 &&
                        sprite_c->sub_texture.uv[1].y != 0 &&
                        sprite_c->sub_texture.uv[2].y != 0 &&
                        sprite_c->sub_texture.uv[3].y != 0;

                    if (has_subtexture)
                    {
                        renderer_draw_sub_texture(renderer,
                            sprite_c->texture,
                            &sprite_c->sub_texture,
                            location,
                            trans_c->size,
                            sprite_c->color);
                    }
                    else
                    {
                        renderer_draw_quad(renderer,
                            sprite_c->texture,
                            location,
                            trans_c->size,
                            sprite_c->color);
                    }
                }

            }
        }
    }
}


entity_t ecs_get_entity(ecs_scene_t* scene, char* name)
{
    for (entity_t e = 0; e < scene->entity_count; e++)
    {
        if (strcmp(scene->data_components[e].name, name) == 0)
            return e;
    }
    return -1;
}
void ecs_set_parent(ecs_scene_t* scene, char* name, char* parent)
{
    scene->data_components[ecs_get_entity(scene, name)].parent = ecs_get_entity(scene, parent);
}