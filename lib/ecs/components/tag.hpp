/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tag
*/

#pragma once

#include <concepts>

enum test {
    A,
    B,
};

namespace ecs::component {

template <typename T>
concept Equalable = std::equality_comparable<T>;

/**
 * @brief Represents a `tag` an entity can have.
 *
 * The `tag` component is used when you want to mark an entity as part of a 'team'
 * or anything else. It is useful want to create side effects inside systems based on the tag.
 * For example in the collision system, entity with different tags will suffer
 * from some side effects decided by the user.
 */
template <Equalable T>
struct tag {
    T tag;
};

} // namespace ecs::component
