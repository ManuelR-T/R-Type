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
class ZipperIterator {
    public:
    /** @brief The type of value returned by the iterator (tuple of component references). */
    using value_type_t = std::tuple<Components &...>;

    /** @brief The reference type returned by the dereference operator. */
    using reference_t = value_type_t;

    /** @brief The pointer type (void since pointers are not used). */
    using pointer_t = void;

    /** @brief The type used for measuring the distance between iterators. */
    using difference_type_t = std::ptrdiff_t;

    /** @brief The category of the iterator (forward iterator). */
    using iterator_category_t = std::forward_iterator_tag;

    /**
     * @brief Constructs a `zipper_iterator`.
     *
     * @param index The starting index for iteration.
     * @param max_size The maximum size to iterate up to.
     * @param arrays A tuple containing references to the `sparse_array` instances for each component.
     */
    ZipperIterator(size_t index, size_t maxSize, std::tuple<SparseArray<Components> &...> &arrays)
        : _index(index), _maxSize(maxSize), _arrays(arrays)
    {
        _skipInvalid();
    }

    /**
     * @brief Pre-increment operator.
     *
     * Advances the iterator to the next valid entity that possesses all specified components.
     *
     * @return Reference to the incremented iterator.
     */
    ZipperIterator &operator++()
    {
        ++_index;
        _skipInvalid();
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
    bool operator!=(const ZipperIterator &other) const
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
    reference_t operator*()
    {
        return _getComponents(std::index_sequence_for<Components...>{});
    }

    /**
     * @brief Retrieves the current index of the iterator.
     *
     * @return size_t The current index.
     */
    size_t index() const
    {
        return _index;
    }

    private:
    /**
     * @brief Skips over entities that do not possess all specified components.
     *
     * Advances the iterator index until it points to an entity that has all components
     * or until the maximum size is reached.
     */
    void _skipInvalid()
    {
        while (_index < _maxSize && !_allPresent()) {
            ++_index;
        }
    }

    /**
     * @brief Checks if the current entity possesses all specified components.
     *
     * @return `true` if the entity has all components, `false` otherwise.
     */
    bool _allPresent() const
    {
        return _allPresentImpl(std::index_sequence_for<Components...>{});
    }

    /**
     * @brief Helper function to check the presence of components using index sequences.
     *
     * @tparam Is Index sequence for unpacking component types.
     * @param seq The index sequence.
     * @return `true` if all components are present, `false` otherwise.
     */
    template <std::size_t... Is>
    bool _allPresentImpl(std::index_sequence<Is...> seq) const
    {
        return (... && _isPresent<Is>());
    }

    /**
     * @brief Checks if a specific component is present at the current index.
     *
     * @tparam I The index of the component in the tuple.
     * @return `true` if the component is present, `false` otherwise.
     */
    template <std::size_t I>
    bool _isPresent() const
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
    reference_t _getComponents(std::index_sequence<Is...> seq)
    {
        return std::tie(*std::get<Is>(_arrays)[_index]...);
    }

    size_t _index;                                     /**< The current index in the iteration. */
    size_t _maxSize;                                   /**< The maximum index to iterate up to. */
    std::tuple<SparseArray<Components> &...> &_arrays; /**< Tuple of references to component arrays. */
};
} // namespace ecs
