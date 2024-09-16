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

template <typename Component> class sparse_array {
public:
  using value_type = std::optional<Component>;
  using reference_type = value_type &;
  using const_reference_type = const value_type &;
  using container_t = std::vector<value_type>;
  using size_type = typename container_t::size_type;
  using iterator = typename container_t::iterator;
  using const_iterator = typename container_t::const_iterator;

  sparse_array() = default;
  sparse_array(const sparse_array &) = default;
  sparse_array(sparse_array &&) noexcept = default;
  ~sparse_array() = default;

  sparse_array &operator=(const sparse_array &) = default;
  sparse_array &operator=(sparse_array &&) noexcept = default;

  reference_type operator[](size_type idx) {
    if (idx >= _data.size()) {
      throw std::out_of_range("Index out of range in sparse_array");
    }
    return _data[idx];
  }

  const_reference_type operator[](size_type idx) const {
    if (idx >= _data.size()) {
      throw std::out_of_range("Index out of range in sparse_array");
    }
    return _data[idx];
  }

  // Iterators
  iterator begin() { return _data.begin(); }
  const_iterator begin() const { return _data.begin(); }
  const_iterator cbegin() const { return _data.cbegin(); }
  iterator end() { return _data.end(); }
  const_iterator end() const { return _data.end(); }
  const_iterator cend() const { return _data.cend(); }

  size_type size() const { return _data.size(); }

  reference_type insert_at(size_type pos, const Component &component) {
    ensure_size(pos);
    _data[pos] = component;
    return _data[pos];
  }

  reference_type insert_at(size_type pos, Component &&component) {
    ensure_size(pos);
    _data[pos] = std::move(component);
    return _data[pos];
  }

  template <class... Params>
  reference_type emplace_at(size_type pos, Params &&...params) {
    ensure_size(pos);
    _data[pos].emplace(std::forward<Params>(params)...);
    return _data[pos];
  }

  void erase(size_type pos) {
    if (pos < _data.size()) {
      _data[pos].reset();
    }
  }

  size_type get_index(const_reference_type value) const {
    auto it = std::find(_data.begin(), _data.end(), value);
    if (it != _data.end()) {
      return std::distance(_data.begin(), it);
    }
    return _data.size(); // Not found
  }

  bool has(size_type pos) const { return pos < _data.size() && _data[pos]; }

private:
  void ensure_size(size_type pos) {
    if (pos >= _data.size()) {
      _data.resize(pos + 1);
    }
  }

  container_t _data;
};
