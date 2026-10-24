/**
 * @file    commands/primitives.hpp
 * @brief   Vocabulary types and forward declarations of every command.
 *
 * Type tags (With / Include / Exclude / View), the result types (Pool, Atlas,
 * DynamicView), and the declaration of each command, so headers can name them
 * without pulling in their definitions.
 *
 *   With<A, B>      the components a Query fetches (entities must have all of them)
 *   Include<...>    extra components an entity must have, not fetched (tags go here)
 *   Exclude<...>    components an entity must NOT have
 *   View<Inc, Exc>  Include + Exclude as one optional filter argument
 *
 *   Pool<A, B>      references to the components of ONE entity (std::tuple<A&, B&>)
 *   Atlas<A, B>     one Pool per entity
 *   DynamicView     a runtime list of entities
 *
 * Pools and Atlases hold references into component storage: they stay valid
 * until components of that type are added to or removed from any entity.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/meta/types.hpp>

namespace moss {

using Entity = entt::entity;

template<typename... Ts> struct With { };
template<typename... Ts> struct Include { };
template<typename... Ts> struct Exclude { };

using DynamicView = std::vector<Entity>;

template<typename... C> using Pool = std::tuple<C&...>;
template<typename... C> using Atlas = std::vector<Pool<C...>>;

namespace commands {

// A tag, not a command: the optional filter argument of Query
template<typename Inc = Include<>, typename Exc = Exclude<>>
struct View { };

template<typename Wth, typename Vw = View<>> class Query;
template<typename Wth> class Attach;
template<typename Wth> class Detach;
template<typename T> class Resource;
class Create;
class Destroy;
class Quit;

}

namespace detail {

/*
 * Adds T to an entity. Shared by Mirror::attach and cmd::Attach. A component
 * that is already there is replaced with a warning (remove + emplace, so T does
 * not need to be assignable). Returns T&, or void for a tag.
 */
template<typename T, typename... Args>
decltype(auto) put(entt::registry& registry, Entity entity, Args&&... args) {
    if (registry.all_of<T>(entity)) {
        M_WARN("Entity already has component '{}', replacing it", typeid(T).name());
        registry.remove<T>(entity);
    }

    return registry.emplace<T>(entity, std::forward<Args>(args)...);
}

}

}
