/**
 * @file    commands/attach.hpp
 * @brief   Attach: add components to entities.
 *
 *     cmd::Attach<With<Foo>>::init(key).to(entity, 1, 2);     // Foo(1, 2) or aggregate Foo{1, 2}
 *     cmd::Attach<With<Foo>>::init(key).to(entities);         // default Foo on each
 *     cmd::Attach<With<Foo, Tag>>::init(key).to(entity);
 *
 * Every type in With is constructed from the same arguments, so pass arguments
 * only with a single type. The call returns Pool<...> (one entity) or
 * Atlas<...> (a list) of the new components, or void when any of the types is
 * a tag. Attaching a component the entity already has logs a warning and
 * replaces it. The entity must be alive.
 *
 * To attach to everything matching a filter, combine it with a Query:
 *
 *     auto ids = cmd::Query<With<A>, View<Exclude<B>>>::init(key).entities();
 *     cmd::Attach<With<B>>::init(key).to(ids);
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

template<typename... C>
requires(sizeof...(C) > 0)
class Attach<With<C...>> : public Command<Attach<With<C...>>> {
public:
    static constexpr bool hasTag = (std::is_empty_v<C> || ...);

    Attach() = default;

    template<typename... Args>
    auto to(Entity entity, Args&&... args) const {
        entt::registry& reg = this->registry();
        M_FATAL_IFF(!reg.valid(entity), "Attach::to: the entity is not valid");

        if constexpr (hasTag) {
            ((void)detail::put<C>(reg, entity, args...), ...);
        } else {
            return Pool<C...>(detail::put<C>(reg, entity, args...)...);
        }
    }

    template<typename... Args>
    auto to(const DynamicView& entities, Args&&... args) const {
        if constexpr (hasTag) {
            for (Entity entity : entities) to(entity, args...);
        } else {
            Atlas<C...> out;
            out.reserve(entities.size());
            for (Entity entity : entities) out.push_back(to(entity, args...));
            return out;
        }
    }
};

}
