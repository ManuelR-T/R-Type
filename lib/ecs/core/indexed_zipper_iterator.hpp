/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** indexed_zipper_iterator
*/

#include "core/zipper_iterator.hpp"

namespace ecs {

template <typename... Components>
class indexed_zipper_iterator {
    public:
    using base_iterator = zipper_iterator<Components...>;
    using value_type = std::tuple<size_t, Components &...>;
    using reference = value_type;

    indexed_zipper_iterator(base_iterator base_it) : _base_it(base_it) {}

    indexed_zipper_iterator &operator++()
    {
        ++_base_it;
        return *this;
    }

    bool operator!=(const indexed_zipper_iterator &other) const
    {
        return _base_it != other._base_it;
    }

    reference operator*()
    {
        size_t index = _base_it.index();
        auto components = *_base_it;
        return std::tuple_cat(std::make_tuple(index), components);
    }

    private:
    base_iterator _base_it;
};
} // namespace ecs
