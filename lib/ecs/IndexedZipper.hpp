/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** indexed_zipper
*/

#pragma once

#include "IndexedZipperIterator.hpp"
#include "Zipper.hpp"

namespace ecs {

/**
 * @class IndexedZipper
 * @brief A class template that provides indexed iteration over multiple SparseArray components.
 *
 * @tparam Components The component types to be iterated over.
 */
template <typename... Components>
class IndexedZipper {
    public:
    /**
     * @typedef base_zipper_t
     * @brief Alias for the base zipper type.
     */
    using base_zipper_t = ecs::Zipper<Components...>;

    /**
     * @typedef iterator_t
     * @brief Alias for the iterator type.
     */
    using iterator_t = IndexedZipperIterator<Components...>;

    /**
     * @brief Constructs an IndexedZipper with the given SparseArray components.
     *
     * @param arrays The SparseArray components to be iterated over.
     */
    IndexedZipper(ecs::SparseArray<Components> &...arrays);

    /**
     * @brief Returns an iterator to the beginning of the zipped components.
     *
     * @return iterator_t An iterator to the beginning.
     */
    iterator_t begin();

    /**
     * @brief Returns an iterator to the end of the zipped components.
     *
     * @return iterator_t An iterator to the end.
     */
    iterator_t end();

    private:
    /**
     * @brief The base zipper that handles the actual iteration logic.
     */
    base_zipper_t _baseZipper;
};

} // namespace ecs
