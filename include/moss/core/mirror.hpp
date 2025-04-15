/**
 * @file    core/mirror.hpp
 * @brief   Provides the Mirror class for managing entities and systems.
 *
 * The `Mirror` class is responsible for creating and attaching entities and systems
 * to the `Context`. It also provides a means to interact with the ECS registry
 * during the `init()` phase of the `Context` lifecycle. It serves as the primary 
 * interface for modifying and managing entities and systems.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/core/context.hpp>
#include <moss/ecs/component.hpp>
#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>


namespace moss {

/**
 * @class Mirror
 * @brief Utility class for managing entities and systems in a Context.
 *
 * The `Mirror` class is responsible for creating and managing entities and attaching
 * components or systems to those entities within the scope of a `Context`.
 * It provides a fluent interface for adding entities, components, and systems
 * to a `Context`. The `create()`, `attach()`, and `connect()` methods offer
 * the necessary abstractions for entity and system management.
 *
 * The `Mirror` is used internally by `Context` to register components and systems
 * and should not be used directly by users outside of the `Context`'s `init()` phase.
 */
class Mirror {
public:
    /**
     * @brief Creates a set of new entities.
     *
     * This function creates `count` number of new entities and adds them to the
     * `Mirror`'s entity view. These entities are then available for attaching components
     * or systems.
     *
     * @param count Number of entities to create (defaults to 1).
     * @return Reference to this Mirror instance.
     */
    Mirror& create(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            entt::entity e = m_registry->create();
            m_view.push_back(e); m_contex.m_view.push_back(e);
        }

        return *this;
    }

    /**
     * @brief Attaches components to entities in the Mirror's view.
     *
     * This method attaches one or more components to each of the entities in the
     * `m_view`. Components must inherit from `moss::Component`. The components are 
     * initialized when attached.
     *
     * @tparam T Components to attach to each entity.
     * @return Reference to this Mirror instance.
     */
    template<typename... T>
    Mirror& attach() {
        static_assert(
            std::is_base_of<Component, T...>::value,
            "Expected all of T to inherit moss::Component"
        );

        for (entt::entity entity : m_view)
            m_registry->emplace<T...>(entity).init();

        return *this;
    }

    /**
     * @brief Connects systems to the Context.
     *
     * This method registers one or more systems with the `Context`. Systems must inherit
     * from `moss::System`. The systems are added to the `Context`'s system view.
     *
     * @tparam T Systems to be connected to the Context.
     * @return Reference to this Mirror instance.
     */
    template<typename... T>
    Mirror& connect() {
        static_assert(
            std::is_base_of<System, T...>::value,
            "Expected all of T to inherit moss::System"
        );

        m_contex.m_view.push_back(T()...);
        return *this;
    }


private:
    friend class App;
    Mirror() { }

    Context m_contex;
    DynamicView m_view;
    entt::registry* m_registry;
};

}
