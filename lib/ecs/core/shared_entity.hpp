/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shared_entity
*/

#include <random>

/**
 * @typedef shared_entity_t
 * @brief Alias for the shared entity identifier type.
 *
 * `shared_entity_t` is defined as `std::size_t` to uniquely identify entities within the network.
 */
using shared_entity_t = std::size_t;

namespace ecs {
    shared_entity_t generate_shared_entity_id();
} // namespace ecs