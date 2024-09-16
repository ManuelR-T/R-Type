/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** entity
*/

#pragma once

#include <cstddef>
#include <queue>

using entity_t = std::size_t;

class EntityManager {
    public:
    entity_t create_entity()
    {
        if (!dead_entities.empty()) {
            entity_t id = dead_entities.front();
            dead_entities.pop();
            return id;
        }
        return next_entity_id++;
    }

    void destroy_entity(entity_t entity)
    {
        dead_entities.push(entity);
    }

    private:
    entity_t next_entity_id = 0;
    std::queue<entity_t> dead_entities;
};
