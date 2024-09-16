/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** zipper_iterator
*/

#pragma once

#include "sparse_array.hpp"
#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>

template <typename... Components> class zipper_iterator {
public:
  using value_type = std::tuple<Components &...>;
  using reference = value_type;
  using pointer = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  zipper_iterator(size_t index, size_t max_size,
                  std::tuple<sparse_array<Components> &...> &arrays)
      : _index(index), _max_size(max_size), _arrays(arrays) {
    skip_invalid();
  }

  zipper_iterator &operator++() {
    ++_index;
    skip_invalid();
    return *this;
  }

  bool operator!=(const zipper_iterator &other) const {
    return _index != other._index;
  }

  reference operator*() {
    return get_components(std::index_sequence_for<Components...>{});
  }

private:
  void skip_invalid() {
    while (_index < _max_size && !all_present()) {
      ++_index;
    }
  }

  bool all_present() const {
    return all_present_impl(std::index_sequence_for<Components...>{});
  }

  template <std::size_t... Is>
  bool all_present_impl(std::index_sequence<Is...>) const {
    return (... && is_present<Is>());
  }

  template <std::size_t I> bool is_present() const {
    const auto &array = std::get<I>(_arrays);
    if (_index >= array.size()) {
      return false;
    }
    return array[_index].has_value();
  }

  template <std::size_t... Is>
  reference get_components(std::index_sequence<Is...>) {
    return std::tie(*std::get<Is>(_arrays)[_index]...);
  }

  size_t _index;
  size_t _max_size;
  std::tuple<sparse_array<Components> &...> &_arrays;
};
