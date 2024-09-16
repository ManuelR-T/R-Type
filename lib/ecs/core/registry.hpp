/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** registry
*/

#pragma once

#include "entity.hpp"
#include "sparse_array.hpp"

#include <any>
#include <functional>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

class registry {
    public:
    registry() = default;
    ~registry() = default;

    template <typename Component>
    sparse_array<Component> &register_component()
    {
        std::type_index typeIdx(typeid(Component));
        if (_components_arrays.find(typeIdx) == _components_arrays.end()) {
            _components_arrays[typeIdx] = sparse_array<Component>();
            _erase_functions[typeIdx] = [](registry &reg, const entity_t &entity) {
                reg.get_components<Component>().erase(static_cast<size_t>(entity));
            };
        }
        return std::any_cast<sparse_array<Component> &>(_components_arrays[typeIdx]);
    }

    template <typename Component>
    sparse_array<Component> &get_components()
    {
        std::type_index typeIdx(typeid(Component));
        auto it = _components_arrays.find(typeIdx);
        if (it == _components_arrays.end()) {
            throw std::runtime_error("Component not registered.");
        }
        return std::any_cast<sparse_array<Component> &>(it->second);
    }

    template <typename Component>
    const sparse_array<Component> &get_components() const
    {
        std::type_index typeIdx(typeid(Component));
        auto it = _components_arrays.find(typeIdx);
        if (it == _components_arrays.end()) {
            throw std::runtime_error("Component not registered.");
        }
        return std::any_cast<const sparse_array<Component> &>(it->second);
    }

    entity_t spawn_entity()
    {
        entity_t entity(_next_entity_id++);
        _entities.push_back(entity);
        return entity;
    }

    void kill_entity(const entity_t &entity)
    {
        for (auto &[typeIdx, eraseFunc] : _erase_functions) {
            eraseFunc(*this, entity);
        }
        _entities_to_destroy.push_back(entity);
    }

    template <typename Component>
    typename sparse_array<Component>::reference_type add_component(const entity_t &entity, Component &&component)
    {
        auto &array = get_components<Component>();
        return array.insert_at(static_cast<size_t>(entity), std::forward<Component>(component));
    }

    template <typename Component, typename... Params>
    typename sparse_array<Component>::value_type &emplace_component(const entity_t &entity, Params &&...params)
    {
        auto &array = get_components<Component>();
        return array.emplace_at(static_cast<size_t>(entity), std::forward<Params>(params)...);
    }

    template <typename Component>
    void remove_component(const entity_t &entity)
    {
        auto &array = get_components<Component>();
        array.erase(static_cast<size_t>(entity));
    }

    template <typename Function>
    void add_system(Function &&system)
    {
        _systems.emplace_back(std::forward<Function>(system));
    }

    void run_systems()
    {
        for (auto &system : _systems) {
            system();
        }
    }

    private:
    std::unordered_map<std::type_index, std::any> _components_arrays;
    std::unordered_map<std::type_index, std::function<void(registry &, const entity_t &)>> _erase_functions;
    std::vector<std::function<void()>> _systems;
    std::vector<entity_t> _entities;
    std::vector<entity_t> _entities_to_destroy;
    std::size_t _next_entity_id = 0;
};
