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
class IndexedZipper {
    public:
    using base_zipper_t = ecs::Zipper<Components...>;
    using iterator_t = IndexedZipperIterator<Components...>;

    IndexedZipper(ecs::SparseArray<Components> &...arrays) : _baseZipper(arrays...) {}

    iterator_t begin()
    {
        return iterator_t(_baseZipper.begin());
    }

    iterator_t end()
    {
        return iterator_t(_baseZipper.end());
    }

    private:
    base_zipper_t _baseZipper;
};
} // namespace ecs
