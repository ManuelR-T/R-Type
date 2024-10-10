/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** indexed_zipper_iterator
*/

#include "ZipperIterator.hpp"

namespace ecs {
/**
 * @class IndexedZipperIterator
 * @brief An iterator that zips multiple component iterators and provides an index for each element.
 *
 * @tparam Components The types of the components to be iterated over.
 */
template <typename... Components>
class IndexedZipperIterator {
    public:
    /**
     * @typedef base_iterator_t
     * @brief The base iterator type, which is a ZipperIterator over the specified components.
     */
    using base_iterator_t = ZipperIterator<Components...>;
    /**
     * @typedef value_type_t
     * @brief The type of the value returned by the iterator, which is a tuple containing an index and references to the
     * components.
     */
    using value_type_t = std::tuple<size_t, Components &...>;

    /**
     * @typedef reference_t
     * @brief The reference type, which is the same as value_type_t.
     */
    using reference_t = value_type_t;

    /**
     * @brief Constructs an IndexedZipperIterator with a base iterator.
     *
     * @param baseIt The base iterator to be wrapped.
     */
    IndexedZipperIterator(base_iterator_t baseIt) : _baseIt(baseIt) {}

    /**
     * @brief Advances the iterator to the next element.
     *
     * @return A reference to the incremented iterator.
     */
    IndexedZipperIterator &operator++()
    {
        ++_baseIt;
        return *this;
    }

    /**
     * @brief Compares two IndexedZipperIterator objects for inequality.
     *
     * @param other The other iterator to compare with.
     * @return True if the iterators are not equal, false otherwise.
     */
    bool operator!=(const IndexedZipperIterator &other) const
    {
        return _baseIt != other._baseIt;
    }

    /**
     * @brief Dereferences the iterator to access the current element.
     *
     * @return A tuple containing the index and references to the components of the current element.
     */
    reference_t operator*()
    {
        size_t index = _baseIt.index();
        auto components = *_baseIt;
        return std::tuple_cat(std::make_tuple(index), components);
    }

    private:
    base_iterator_t _baseIt; ///< The base iterator being wrapped.
};
} // namespace ecs
