/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** indexed_zipper
*/

#pragma once

#include "core/zipper.hpp"
#include "core/indexed_zipper_iterator.hpp"

namespace ecs {
template <typename... Components>
class indexed_zipper {
    public:
    using base_zipper = ecs::zipper<Components...>;
    using iterator = indexed_zipper_iterator<Components...>;

    indexed_zipper(ecs::sparse_array<Components> &...arrays) : _base_zipper(arrays...) {}

    iterator begin()
    {
        return iterator(_base_zipper.begin());
    }

    iterator end()
    {
        return iterator(_base_zipper.end());
    }

    private:
    base_zipper _base_zipper;
};
} // namespace ecs
