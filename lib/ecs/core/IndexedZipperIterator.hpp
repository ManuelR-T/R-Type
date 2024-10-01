/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** indexed_zipper_iterator
*/

#include "core/ZipperIterator.hpp"

namespace ecs {

template <typename... Components>
class IndexedZipperIterator {
    public:
    using base_iterator_t = ZipperIterator<Components...>;
    using value_type_t = std::tuple<size_t, Components &...>;
    using reference_t = value_type_t;

    IndexedZipperIterator(base_iterator_t baseIt) : _baseIt(baseIt) {}

    IndexedZipperIterator &operator++()
    {
        ++_baseIt;
        return *this;
    }

    bool operator!=(const IndexedZipperIterator &other) const
    {
        return _baseIt != other._baseIt;
    }

    reference_t operator*()
    {
        size_t index = _baseIt.index();
        auto components = *_baseIt;
        return std::tuple_cat(std::make_tuple(index), components);
    }

    private:
    base_iterator_t _baseIt;
};
} // namespace ecs
