/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** zipper
*/

#pragma once

#include <algorithm>
#include <tuple>
#include "sparse_array.hpp"
#include "zipper_iterator.hpp"

/**
 * @brief Facilitates iteration over entities possessing all specified components.
 *
 * The `zipper` class template provides an interface to iterate through entities that have
 * all of the specified component types. It leverages `zipper_iterator` to traverse the
 * components in a synchronized manner, ensuring that only entities with complete component sets
 * are accessed.
 *
 * @tparam Components The types of components to include in the iteration.
 */
template <typename... Components>
class zipper {
    public:
    /** @brief Type alias for the zipper iterator. */
    using iterator = zipper_iterator<Components...>;

    /**
     * @brief Constructs a `zipper` with references to component arrays.
     *
     * Initializes the `zipper` with the provided `sparse_array` instances for each component type.
     * These arrays are used by the iterator to traverse entities that have all specified components.
     *
     * @param arrays References to `sparse_array` instances for each component type.
     */
    zipper(sparse_array<Components> &...arrays) : _arrays(arrays...) {}

    /**
     * @brief Returns an iterator to the beginning of the zipper.
     *
     * Creates a `zipper_iterator` starting at index `0` and iterates up to the maximum size
     * among all component arrays. Only entities that have all specified components will be
     * included in the iteration.
     *
     * @return An `iterator` pointing to the first valid entity.
     */
    iterator begin()
    {
        size_t max_size = get_max_size();
        return iterator(0, max_size, _arrays);
    }

    /**
     * @brief Returns an iterator to the end of the zipper.
     *
     * Creates a `zipper_iterator` pointing to the end of the iteration range. This iterator
     * signifies the termination of the iteration process.
     *
     * @return An `iterator` representing the end of the iteration range.
     */
    iterator end()
    {
        size_t max_size = get_max_size();
        return iterator(max_size, max_size, _arrays);
    }

    private:
    /**
     * @brief Determines the maximum size among all component arrays.
     *
     * Calculates the largest size from the provided `sparse_array` instances. This value
     * defines the upper bound for the iteration range.
     *
     * @return The maximum size (`size_t`) among all component arrays.
     */
    size_t get_max_size() const
    {
        return std::max({std::get<sparse_array<Components> &>(_arrays).size()...});
    }

    /**
     * @brief Tuple of references to component arrays.
     *
     * Stores references to each `sparse_array` corresponding to the component types. This
     * tuple is used by the `zipper_iterator` to access and traverse the components.
     */
    std::tuple<sparse_array<Components> &...> _arrays;
};
