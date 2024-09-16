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

template <typename... Components>
class zipper {
    public:
    using iterator = zipper_iterator<Components...>;

    zipper(sparse_array<Components> &...arrays) : _arrays(arrays...) {}

    iterator begin()
    {
        size_t max_size = get_max_size();
        return iterator(0, max_size, _arrays);
    }

    iterator end()
    {
        size_t max_size = get_max_size();
        return iterator(max_size, max_size, _arrays);
    }

    private:
    size_t get_max_size() const
    {
        return std::max({std::get<sparse_array<Components> &>(_arrays).size()...});
    }

    std::tuple<sparse_array<Components> &...> _arrays;
};
