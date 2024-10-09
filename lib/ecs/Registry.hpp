/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** registry
*/

#pragma once

#include "SparseArray.hpp"
#include "entity.hpp"
#include "components/shared_entity.hpp"
#include "shared_entity.hpp"

#include <any>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
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
class Registry {
    public:
    /**
     * @brief Default constructor for the registry.
     *
     * Initializes the registry with no entities, components, or systems.
     */
    Registry() = default;

    /**
     * @brief Default destructor for the registry.
     *
     * Cleans up any resources used by the registry.
     */
    ~Registry() = default;

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
    SparseArray<Component> &registerComponent()
    {
        std::type_index typeIdx(typeid(Component));
        if (_componentsArrays.find(typeIdx) == _componentsArrays.end()) {
            _componentsArrays[typeIdx] = SparseArray<Component>();
            _eraseFunctions[typeIdx] = [](Registry &reg, const entity_t &entity) {
                reg.getComponents<Component>().erase(static_cast<std::size_t>(entity));
            };
        }
        return std::any_cast<SparseArray<Component> &>(_componentsArrays[typeIdx]);
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
    SparseArray<Component> &getComponents()
    {
        std::type_index typeIdx(typeid(Component));
        auto it = _componentsArrays.find(typeIdx);
        if (it == _componentsArrays.end()) {
            throw std::runtime_error(std::string("Component not registered: ") + typeid(Component).name());
        }
        return std::any_cast<SparseArray<Component> &>(it->second);
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
    const SparseArray<Component> &getComponents() const
    {
        std::type_index typeIdx(typeid(Component));
        auto it = _componentsArrays.find(typeIdx);
        if (it == _componentsArrays.end()) {
            throw std::runtime_error(std::string("Component not registered: ") + typeid(Component).name());
        }
        return std::any_cast<const SparseArray<Component> &>(it->second);
    }

    /**
     * @brief Spawns a new entity within the registry.
     *
     * Creates a new entity identifier, adds it to the list of active entities, and returns
     * the identifier.
     *
     * @return The identifier (`entity_t`) of the newly spawned entity.
     */
    entity_t spawnEntity()
    {
        entity_t entity(_nextEntityId++);
        _entities.push_back(entity);
        return entity;
    }

    /**
     * @brief Spawns a new shared_entity within the registry.
     *
     * Creates a new sharedentity identifier, adds it to the list of active entities, and returns
     * the identifier.
     *
     * @param shared_entity_id The id of the shared_entity.
     * @return The identifier (`entity_t`) of the newly spawned sharedentity.
     */
    entity_t spawnSharedEntity(shared_entity_t sharedEntityId)
    {
        entity_t entity(_nextEntityId++);
        _entities.push_back(entity);

        _sharedEntityTracker[sharedEntityId] = entity;

        this->addComponent(entity, ecs::component::SharedEntity{sharedEntityId});

        return entity;
    }

    const std::unordered_map<shared_entity_t, entity_t> &getLocalEntity() const
    {
        return _sharedEntityTracker;
    }

    /**
     * @brief Marks an entity for destruction.
     *
     * Removes all components associated with the specified entity and queues the entity
     * for destruction, making its identifier available for reuse.
     *
     * @param entity The identifier (`entity_t`) of the entity to destroy.
     */
    void killEntity(const entity_t &entity)
    {
        for (auto &[typeIdx, eraseFunc] : _eraseFunctions) {
            eraseFunc(*this, entity);
        }
        _entitiesToDestroy.push_back(entity);
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
    typename SparseArray<Component>::reference_type_t addComponent(const entity_t &entity, Component &&component)
    {
        auto &array = getComponents<Component>();
        return array.insertAt(static_cast<std::size_t>(entity), std::forward<Component>(component));
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
    typename SparseArray<Component>::value_type_t &emplaceComponent(const entity_t &entity, Params &&...params)
    {
        auto &array = getComponents<Component>();
        return array.emplace_at(static_cast<std::size_t>(entity), std::forward<Params>(params)...);
    }

    template <typename Component>
    typename SparseArray<Component>::reference_type_t getComponent(const entity_t &entity)
    {
        auto &array = getComponents<Component>();
        if (!array.has(static_cast<size_t>(entity))) {
            throw std::runtime_error("Component not found for this entity.");
        }
        return array[static_cast<size_t>(entity)];
    }

    template <typename Component>
    typename SparseArray<Component>::const_reference_type_t getComponent(const entity_t &entity) const
    {
        const auto &array = getComponents<Component>();
        if (!array.has(static_cast<size_t>(entity))) {
            throw std::runtime_error("Component not found for this entity.");
        }
        return array[static_cast<size_t>(entity)];
    }

    /**
     * @brief Checks if an entity has a specific component.
     *
     * This template function checks if the specified entity has the specified component
     * by querying the corresponding `sparse_array`.
     * @tparam Component The type of the component to check for.
     * @param entity The identifier (`entity_t`) of the entity.
     * @return `true` if the entity has the component, `false` otherwise.
     */
    template <typename Component>
    bool hasComponent(const entity_t &entity) const
    {
        const auto &array = getComponents<Component>();
        return array.has(static_cast<size_t>(entity));
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
    void removeComponent(const entity_t &entity)
    {
        auto &array = getComponents<Component>();
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
    void addSystem(Function &&system)
    {
        _systems.emplace_back(std::forward<Function>(system));
    }

    /**
     * @brief Executes all registered systems.
     *
     * Iterates through all added systems and executes each one in sequence. Systems typically
     * contain the logic to process entities and their components.
     */
    void runSystems()
    {
        for (auto &system : _systems) {
            system();
        }
    }

    private:
    std::unordered_map<std::type_index, std::any>
        _componentsArrays; /**< Stores component arrays indexed by their type */
    std::unordered_map<std::type_index, std::function<void(Registry &, const entity_t &)>>
        _eraseFunctions;                         /**< Stores erase functions for each component type */
    std::vector<std::function<void()>> _systems; /**< List of systems to be executed */
    std::vector<entity_t> _entities;             /**< List of active entities */
    std::vector<entity_t> _entitiesToDestroy;    /**< Queue of entities marked for destruction */
    std::size_t _nextEntityId = 0;               /**< The next available entity identifier */

    std::unordered_map<shared_entity_t, entity_t> _sharedEntityTracker;
};

} // namespace ecs
