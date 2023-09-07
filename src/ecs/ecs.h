#pragma once

#define MAX_ENTITY_COUNT 1000
typedef i32 entity_t;

#include "ecs/components.h"

struct ecs_t
{
    u32 entity_count;
    data_component_t data_components[MAX_ENTITY_COUNT];
    transform_component_t transform_components[MAX_ENTITY_COUNT];
    camera_component_t camera_components[MAX_ENTITY_COUNT];
    sprite_component_t sprite_components[MAX_ENTITY_COUNT];
    physics_component_t physics_components[MAX_ENTITY_COUNT];
    script_component_t script_components[MAX_ENTITY_COUNT];
};

struct ecs_scene_t
{
    char name[32];
    u32 entity_count;
    data_component_t data_components[MAX_ENTITY_COUNT];
    transform_component_t transform_components[MAX_ENTITY_COUNT];
    camera_component_t camera_components[MAX_ENTITY_COUNT];
    sprite_component_t sprite_components[MAX_ENTITY_COUNT];
    physics_component_t physics_components[MAX_ENTITY_COUNT];
    script_component_t script_components[MAX_ENTITY_COUNT];

    ecs_t* ecs;
    entity_t active_camera;
};

entity_t ecs_create_entity(ecs_t* ecs, char* name);
ecs_scene_t ecs_create_scene(ecs_t* ecs, char* name);
void ecs_reset_entity_values(ecs_scene_t* scene, entity_t e);

void ecs_scene_add_entity(ecs_scene_t* scene, entity_t e, char* name);
void ecs_delete_entity(ecs_scene_t* scene, entity_t entity);

void ecs_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer);

entity_t ecs_get_entity(ecs_scene_t* scene, char* name);
void ecs_set_parent(ecs_scene_t* scene, char* name, char* parent);
#include "ecs/serial.h"