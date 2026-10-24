/**
 * @file    core/mirror.hpp
 * @brief   Mirror: the fluent builder used inside Context::init.
 *
 *     mirror
 *         .connect<A, B, C>()                          systems, in order
 *         .create()                                    a new entity (selection)
 *             .attach<Foo>({.x = 1})                   designated init
 *             .attach<Bar>(1, 2)                       constructor / positional init
 *             .attach<Tag>()                           default
 *         .create(3).attach<Foo, Bar>();               three entities, defaults
 *
 * attach works on the entities of the latest create() and aborts if there is
 * none. Components are plain structs (no base class). The braced form needs an
 * aggregate; the argument form works with any constructor. With several types
 * and arguments, every type is constructed from the same arguments. Attaching a
 * type an entity already has logs a warning and replaces it.
 *
 * Only App makes Mirrors. Use it during init, not at runtime: systems use
 * the commands (cmd::Create, cmd::Attach, ...) instead.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/meta/types.hpp>
#include <moss/commands/primitives.hpp>
#include <moss/core/context.hpp>

namespace moss {

class Mirror {
public:
    // Creates `count` entities and makes them the current selection
    Mirror& create(u32 count = 1) {
        M_FATAL_IFF(count == 0, "Mirror::create: the count must be at least 1");

        m_view.clear();
        m_view.reserve(count);

        for (u32 i = 0; i < count; i++) {
            Entity entity = m_registry->create();
            m_view.push_back(entity);
            m_context->m_view.push_back(entity);
        }

        return *this;
    }

    // Zero or more constructor arguments, one or more types
    template<typename... T, typename... Args>
    requires(sizeof...(T) > 0 && (std::is_class_v<T> && ...))
    Mirror& attach(Args&&... args) {
        requireSelection("attach");

        for (Entity entity : m_view)
            ((void)detail::put<T>(*m_registry, entity, args...), ...);

        return *this;
    }

    // One braced list: .attach<Foo>({.x = 1})
    template<typename T>
    requires(std::is_class_v<T>)
    Mirror& attach(const T& value) {
        requireSelection("attach");

        for (Entity entity : m_view)
            detail::put<T>(*m_registry, entity, value);

        return *this;
    }

    // Systems are default-constructed and run in the order they are connected
    template<typename... T>
    requires(sizeof...(T) > 0 && (std::derived_from<T, System> && ...))
    Mirror& connect() {
        (m_context->m_systems.push_back(std::make_unique<T>()), ...);
        return *this;
    }

private:
    friend class App;

    Mirror(entt::registry* registry, Context* context)
        : m_registry(registry), m_context(context) { }

    void requireSelection(const char* what) const {
        M_FATAL_IF(m_view.empty(), "Mirror::{} called before create()", what);
    }

    entt::registry* m_registry;
    Context* m_context;
    DynamicView m_view;   // entities of the latest create()
};

}
