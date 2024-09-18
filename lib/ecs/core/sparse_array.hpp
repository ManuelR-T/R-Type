/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** sparse_array
*/

#pragma once

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <vector>

namespace ecs {
/**
 * @brief A sparse array implementation for managing components in an ECS framework.
 *
 * The `sparse_array` class provides an efficient way to store and manage components
 * associated with entities in an Entity-Component-System (ECS) architecture. It uses
 * a vector of `std::optional` to allow for sparse storage, enabling quick access and
 * modification of components.
 * TODO: Refactor using bitfield for better performance et remove `std::optional`.
 *
 * @tparam Component The type of component to be stored in the sparse array.
 */
template <typename Component>
class sparse_array {
    public:
    /** @brief Alias for the optional component type. */
    using value_type = std::optional<Component>;

    /** @brief Alias for a reference to the optional component type. */
    using reference_type = value_type &;

    /** @brief Alias for a const reference to the optional component type. */
    using const_reference_type = const value_type &;

    /** @brief Alias for the underlying container type (vector of optional components). */
    using container_t = std::vector<value_type>;

    /** @brief Alias for the size type used by the container. */
    using size_type = typename container_t::size_type;

    /** @brief Alias for the iterator type of the container. */
    using iterator = typename container_t::iterator;

    /** @brief Alias for the const iterator type of the container. */
    using const_iterator = typename container_t::const_iterator;

    /**
     * @brief Default constructor for sparse_array.
     *
     * Initializes an empty sparse array.
     */
    sparse_array() = default;

    /**
     * @brief Default copy constructor.
     *
     * Performs a shallow copy of the sparse array.
     */
    sparse_array(const sparse_array &) = default;

    /**
     * @brief Default move constructor.
     *
     * Moves the contents of another sparse array to this one.
     *
     * @param other The sparse array to move from.
     */
    sparse_array(sparse_array &&other) noexcept = default;

    /**
     * @brief Default destructor.
     *
     * Cleans up the sparse array.
     */
    ~sparse_array() = default;

    /**
     * @brief Default copy assignment operator.
     *
     * Performs a shallow copy assignment.
     *
     * @param other The sparse array to copy from.
     * @return Reference to this sparse array.
     */
    sparse_array &operator=(const sparse_array &other) = default;

    /**
     * @brief Default move assignment operator.
     *
     * Moves the contents of another sparse array to this one.
     *
     * @param other The sparse array to move from.
     * @return Reference to this sparse array.
     */
    sparse_array &operator=(sparse_array &&other) noexcept = default;

    /**
     * @brief Accesses the component at the specified index.
     *
     * Provides mutable access to the component at the given index. Throws an exception
     * if the index is out of range.
     *
     * @param idx The index of the component to access.
     * @return Reference to the optional component at the specified index.
     * @throws std::out_of_range If the index is out of bounds.
     */
    reference_type operator[](size_type idx)
    {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of range in sparse_array");
        }
        return _data[idx];
    }

    /**
     * @brief Accesses the component at the specified index (const version).
     *
     * Provides read-only access to the component at the given index. Throws an exception
     * if the index is out of range.
     *
     * @param idx The index of the component to access.
     * @return Const reference to the optional component at the specified index.
     * @throws std::out_of_range If the index is out of bounds.
     */
    const_reference_type operator[](size_type idx) const
    {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of range in sparse_array");
        }
        return _data[idx];
    }

    /**
     * @brief Returns an iterator to the beginning of the sparse array.
     *
     * @return Iterator pointing to the first element of the sparse array.
     */
    iterator begin()
    {
        return _data.begin();
    }

    /**
     * @brief Returns a const iterator to the beginning of the sparse array.
     *
     * @return Const iterator pointing to the first element of the sparse array.
     */
    const_iterator begin() const
    {
        return _data.begin();
    }

    /**
     * @brief Returns a const iterator to the beginning of the sparse array.
     *
     * @return Const iterator pointing to the first element of the sparse array.
     */
    const_iterator cbegin() const
    {
        return _data.cbegin();
    }

    /**
     * @brief Returns an iterator to the end of the sparse array.
     *
     * @return Iterator pointing to one past the last element of the sparse array.
     */
    iterator end()
    {
        return _data.end();
    }

    /**
     * @brief Returns a const iterator to the end of the sparse array.
     *
     * @return Const iterator pointing to one past the last element of the sparse array.
     */
    const_iterator end() const
    {
        return _data.end();
    }

    /**
     * @brief Returns a const iterator to the end of the sparse array.
     *
     * @return Const iterator pointing to one past the last element of the sparse array.
     */
    const_iterator cend() const
    {
        return _data.cend();
    }

    /**
     * @brief Returns the number of elements in the sparse array.
     *
     * @return The size of the sparse array.
     */
    size_type size() const
    {
        return _data.size();
    }

    /**
     * @brief Inserts a component at the specified position.
     *
     * Inserts a copy of the component at the given position. If the position exceeds the
     * current size, the array is resized accordingly.
     *
     * @param pos The position at which to insert the component.
     * @param component The component to insert.
     * @return Reference to the inserted optional component.
     */
    reference_type insert_at(size_type pos, const Component &component)
    {
        ensure_size(pos);
        _data[pos] = component;
        return _data[pos];
    }

    /**
     * @brief Inserts a component at the specified position using move semantics.
     *
     * Inserts a moved component at the given position. If the position exceeds the
     * current size, the array is resized accordingly.
     *
     * @param pos The position at which to insert the component.
     * @param component The component to insert.
     * @return Reference to the inserted optional component.
     */
    reference_type insert_at(size_type pos, Component &&component)
    {
        ensure_size(pos);
        _data[pos] = std::move(component);
        return _data[pos];
    }

    /**
     * @brief Constructs and inserts a component at the specified position.
     *
     * Constructs a component in-place at the given position using the provided parameters.
     * If the position exceeds the current size, the array is resized accordingly.
     *
     * @tparam Params The types of the parameters to forward to the component's constructor.
     * @param pos The position at which to emplace the component.
     * @param params The parameters to forward to the component's constructor.
     * @return Reference to the emplaced optional component.
     */
    template <class... Params>
    reference_type emplace_at(size_type pos, Params &&...params)
    {
        ensure_size(pos);
        _data[pos].emplace(std::forward<Params>(params)...);
        return _data[pos];
    }

    /**
     * @brief Erases the component at the specified position.
     *
     * Resets the optional component at the given position, effectively removing it.
     *
     * @param pos The position of the component to erase.
     */
    void erase(size_type pos)
    {
        if (pos < _data.size()) {
            _data[pos].reset();
        }
    }

    /**
     * @brief Retrieves the index of a specific component value.
     *
     * Searches for the first occurrence of the specified component value and returns its index.
     * If the value is not found, returns the size of the sparse array.
     *
     * @param value The component value to search for.
     * @return The index of the found component, or the size of the sparse array if not found.
     */
    size_type get_index(const_reference_type value) const
    {
        auto it = std::find(_data.begin(), _data.end(), value);
        if (it != _data.end()) {
            return std::distance(_data.begin(), it);
        }
        return _data.size(); // Not found
    }

    /**
     * @brief Checks if a component exists at the specified position.
     *
     * Determines whether a component is present (i.e., the optional is engaged) at the given
     * position within the sparse array.
     *
     * @param pos The position to check for the presence of a component.
     * @return `true` if a component exists at the position, `false` otherwise.
     */
    bool has(size_type pos) const
    {
        return pos < _data.size() && _data[pos];
    }

    private:
    /**
     * @brief Ensures that the sparse array is large enough to accommodate the specified position.
     *
     * If the current size of the array is less than or equal to the given position, the array is
     * resized to `pos + 1`.
     *
     * @param pos The position that needs to be accommodated within the array.
     */
    void ensure_size(size_type pos)
    {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
    }

    container_t _data; /**< The underlying container storing optional components. */
};

} // namespace ecs
