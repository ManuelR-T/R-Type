/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** zipper_iterator
*/

#pragma once

#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>
#include "sparse_array.hpp"

namespace ecs {
/**
 * @brief An iterator that traverses entities possessing all specified components.
 *
 * The `zipper_iterator` class template provides a way to iterate over entities that have
 * all of the specified component types. It ensures that only entities with complete
 * component sets are accessed, facilitating efficient processing within an ECS framework.
 *
 * @tparam Components The types of components to include in the iteration.
 */
template <typename... Components>
class zipper_iterator {
    public:
    /** @brief The type of value returned by the iterator (tuple of component references). */
    using value_type = std::tuple<Components &...>;

    /** @brief The reference type returned by the dereference operator. */
    using reference = value_type;

    /** @brief The pointer type (void since pointers are not used). */
    using pointer = void;

    /** @brief The type used for measuring the distance between iterators. */
    using difference_type = std::ptrdiff_t;

    /** @brief The category of the iterator (forward iterator). */
    using iterator_category = std::forward_iterator_tag;

    /**
     * @brief Constructs a `zipper_iterator`.
     *
     * @param index The starting index for iteration.
     * @param max_size The maximum size to iterate up to.
     * @param arrays A tuple containing references to the `sparse_array` instances for each component.
     */
    zipper_iterator(size_t index, size_t max_size, std::tuple<sparse_array<Components> &...> &arrays)
        : _index(index), _max_size(max_size), _arrays(arrays)
    {
        skip_invalid();
    }

    /**
     * @brief Pre-increment operator.
     *
     * Advances the iterator to the next valid entity that possesses all specified components.
     *
     * @return Reference to the incremented iterator.
     */
    zipper_iterator &operator++()
    {
        ++_index;
        skip_invalid();
        return *this;
    }

    /**
     * @brief Inequality comparison operator.
     *
     * Compares two iterators for inequality based on their current indices.
     *
     * @param other The other `zipper_iterator` to compare against.
     * @return `true` if the iterators have different indices, `false` otherwise.
     */
    bool operator!=(const zipper_iterator &other) const
    {
        return _index != other._index;
    }

    /**
     * @brief Dereference operator.
     *
     * Accesses the components of the current entity as a tuple of references.
     *
     * @return A tuple containing references to the components of the current entity.
     */
    reference operator*()
    {
        return get_components(std::index_sequence_for<Components...>{});
    }

    private:
    /**
     * @brief Skips over entities that do not possess all specified components.
     *
     * Advances the iterator index until it points to an entity that has all components
     * or until the maximum size is reached.
     */
    void skip_invalid()
    {
        while (_index < _max_size && !all_present()) {
            ++_index;
        }
    }

    /**
     * @brief Checks if the current entity possesses all specified components.
     *
     * @return `true` if the entity has all components, `false` otherwise.
     */
    bool all_present() const
    {
        return all_present_impl(std::index_sequence_for<Components...>{});
    }

    /**
     * @brief Helper function to check the presence of components using index sequences.
     *
     * @tparam Is Index sequence for unpacking component types.
     * @param seq The index sequence.
     * @return `true` if all components are present, `false` otherwise.
     */
    template <std::size_t... Is>
    bool all_present_impl(std::index_sequence<Is...> seq) const
    {
        return (... && is_present<Is>());
    }

    /**
     * @brief Checks if a specific component is present at the current index.
     *
     * @tparam I The index of the component in the tuple.
     * @return `true` if the component is present, `false` otherwise.
     */
    template <std::size_t I>
    bool is_present() const
    {
        const auto &array = std::get<I>(_arrays);
        if (_index >= array.size()) {
            return false;
        }
        return array[_index].has_value();
    }

    /**
     * @brief Retrieves the components of the current entity as a tuple.
     *
     * @tparam Is Index sequence for unpacking component types.
     * @param seq The index sequence.
     * @return A tuple containing references to the components of the current entity.
     */
    template <std::size_t... Is>
    reference get_components(std::index_sequence<Is...> seq)
    {
        return std::tie(*std::get<Is>(_arrays)[_index]...);
    }

    size_t _index;                                      /**< The current index in the iteration. */
    size_t _max_size;                                   /**< The maximum index to iterate up to. */
    std::tuple<sparse_array<Components> &...> &_arrays; /**< Tuple of references to component arrays. */
};
} // namespace ecs
