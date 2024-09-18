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

namespace ecs {
/**
 * @class registry
 * @brief Manages entities, components, and systems within the ECS framework.
 *
 * The `registry` class serves as the core of the ECS (Entity-Component-System) architecture.
 * It handles the creation and destruction of entities, registration and management of components,
 * and the addition and execution of systems.
 */
class registry {
    public:
    /**
     * @brief Default constructor for the registry.
     *
     * Initializes the registry with no entities, components, or systems.
     */
    registry() = default;

    /**
     * @brief Default destructor for the registry.
     *
     * Cleans up any resources used by the registry.
     */
    ~registry() = default;

    /**
     * @brief Registers a new component type.
     *
     * This template function registers a component type within the registry. If the component
     * type is already registered, it simply returns the existing `sparse_array` for that component.
     * Otherwise, it creates a new `sparse_array` and an associated erase function for the component.
     *
     * @tparam Component The type of the component to register.
     * @return A reference to the `sparse_array` managing the registered component type.
     */
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

    /**
     * @brief Retrieves the `sparse_array` for a specific component type.
     *
     * This template function retrieves the `sparse_array` managing the specified component type.
     * If the component type is not registered, it throws a `std::runtime_error`.
     *
     * @tparam Component The type of the component to retrieve.
     * @return A reference to the `sparse_array` managing the specified component type.
     * @throws std::runtime_error If the component type is not registered.
     */
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

    /**
     * @brief Retrieves the `sparse_array` for a specific component type (const version).
     *
     * This template function retrieves the `sparse_array` managing the specified component type.
     * If the component type is not registered, it throws a `std::runtime_error`.
     *
     * @tparam Component The type of the component to retrieve.
     * @return A const reference to the `sparse_array` managing the specified component type.
     * @throws std::runtime_error If the component type is not registered.
     */
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

    /**
     * @brief Spawns a new entity within the registry.
     *
     * Creates a new entity identifier, adds it to the list of active entities, and returns
     * the identifier.
     *
     * @return The identifier (`entity_t`) of the newly spawned entity.
     */
    entity_t spawn_entity()
    {
        entity_t entity(_next_entity_id++);
        _entities.push_back(entity);
        return entity;
    }

    /**
     * @brief Marks an entity for destruction.
     *
     * Removes all components associated with the specified entity and queues the entity
     * for destruction, making its identifier available for reuse.
     *
     * @param entity The identifier (`entity_t`) of the entity to destroy.
     */
    void kill_entity(const entity_t &entity)
    {
        for (auto &[typeIdx, eraseFunc] : _erase_functions) {
            eraseFunc(*this, entity);
        }
        _entities_to_destroy.push_back(entity);
    }

    /**
     * @brief Adds a component to an entity.
     *
     * This template function adds a component to the specified entity by inserting it
     * into the corresponding `sparse_array`.
     *
     * @tparam Component The type of the component to add.
     * @param entity The identifier (`entity_t`) of the entity.
     * @param component The component instance to add to the entity.
     * @return A reference to the inserted component within the `sparse_array`.
     */
    template <typename Component>
    typename sparse_array<Component>::reference_type add_component(const entity_t &entity, Component &&component)
    {
        auto &array = get_components<Component>();
        return array.insert_at(static_cast<size_t>(entity), std::forward<Component>(component));
    }

    /**
     * @brief Constructs and adds a component to an entity in-place.
     *
     * This template function constructs a component directly within the `sparse_array`
     * associated with the specified entity, allowing for efficient initialization.
     *
     * @tparam Component The type of the component to emplace.
     * @tparam Params Parameter pack for the component's constructor.
     * @param entity The identifier (`entity_t`) of the entity.
     * @param params Parameters to forward to the component's constructor.
     * @return A reference to the newly emplaced component within the `sparse_array`.
     */
    template <typename Component, typename... Params>
    typename sparse_array<Component>::value_type &emplace_component(const entity_t &entity, Params &&...params)
    {
        auto &array = get_components<Component>();
        return array.emplace_at(static_cast<size_t>(entity), std::forward<Params>(params)...);
    }

    /**
     * @brief Removes a component from an entity.
     *
     * This template function removes the specified component from the given entity by
     * erasing it from the corresponding `sparse_array`.
     *
     * @tparam Component The type of the component to remove.
     * @param entity The identifier (`entity_t`) of the entity.
     */
    template <typename Component>
    void remove_component(const entity_t &entity)
    {
        auto &array = get_components<Component>();
        array.erase(static_cast<size_t>(entity));
    }

    /**
     * @brief Adds a system to the registry.
     *
     * Systems are functions that operate on entities and their components. This template
     * function adds a new system to the registry's list of systems to be executed.
     *
     * @tparam Function The type of the system function to add.
     * @param system The system function to add.
     */
    template <typename Function>
    void add_system(Function &&system)
    {
        _systems.emplace_back(std::forward<Function>(system));
    }

    /**
     * @brief Executes all registered systems.
     *
     * Iterates through all added systems and executes each one in sequence. Systems typically
     * contain the logic to process entities and their components.
     */
    void run_systems()
    {
        for (auto &system : _systems) {
            system();
        }
    }

    private:
    std::unordered_map<std::type_index, std::any>
        _components_arrays; /**< Stores component arrays indexed by their type */
    std::unordered_map<std::type_index, std::function<void(registry &, const entity_t &)>>
        _erase_functions;                        /**< Stores erase functions for each component type */
    std::vector<std::function<void()>> _systems; /**< List of systems to be executed */
    std::vector<entity_t> _entities;             /**< List of active entities */
    std::vector<entity_t> _entities_to_destroy;  /**< Queue of entities marked for destruction */
    std::size_t _next_entity_id = 0;             /**< The next available entity identifier */
};

} // namespace ecs