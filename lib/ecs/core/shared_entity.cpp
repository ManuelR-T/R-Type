/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shared_entity
*/

#include "shared_entity.hpp"
#include <random>

shared_entity_t ecs::generate_shared_entity_id()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<shared_entity_t> dist(0, std::numeric_limits<shared_entity_t>::max());

    return dist(gen);
}
