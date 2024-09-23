/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_server
*/

#pragma once

void register_components(ecs::registry &reg);
void register_systems(ecs::registry &reg, sf::RenderWindow &window, float &dt);
void register_response(ecs::registry &reg, ecs::response_handler &response_handler);

// ! will be replace by factory
void create_player(ecs::registry &reg, shared_entity_t shared_entity_id);
void create_static(ecs::registry &reg, float x, float y);
// !

