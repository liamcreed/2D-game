#pragma once

#define MAX_ENTITIES 100
typedef u32 entity_t;

#include "ecs/components.h"

typedef struct
{
    u32 entity_count;
    data_component_t data_components[MAX_ENTITIES + 1];
    transform_component_t transform_components[MAX_ENTITIES + 1];
    camera_component_t camera_components[MAX_ENTITIES + 1];
    sprite_component_t sprite_components[MAX_ENTITIES + 1];
    physics_component_t physics_components[MAX_ENTITIES + 1];
    
    entity_t active_camera;
} ecs_scene_t;

void ecs_reset_entity_values(ecs_scene_t* scene, entity_t e);
void ecs_create_scene(ecs_scene_t* scene);
void ecs_update(ecs_scene_t* scene, window_t* window, renderer_t* renderer);
void ecs_exit(ecs_scene_t* scene);

entity_t ecs_create_entity(ecs_scene_t* scene, char* name);
void ecs_delete_entity(ecs_scene_t* scene, entity_t* entity);

#include "ecs/serial.h"