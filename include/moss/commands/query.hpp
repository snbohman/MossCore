/**
 * @file    commands/query.hpp
 * @brief   Query: read components of all entities, a subset, or one entity.
 *
 *     Query<With<A, B>>                         entities that have A and B, fetch both
 *     Query<With<A>, View<Include<Tag>>>        ... that also have Tag (not fetched)
 *     Query<With<A>, View<Include<>, Exclude<X>>>          ... and do not have X
 *
 * With always implies Include. Tags (empty types) cannot be fetched, so they go
 * in View<Include<...>> or are checked with has<T>().
 *
 *     atlas()            every match
 *     atlas(subset)      the entities of `subset` that match, IN THE ORDER of
 *                        `subset` (use the Context's DynamicView to keep creation
 *                        order; atlas() order is entt's pool order)
 *     find(e) / get(e)   one entity: optional Pool / Pool (aborts on no match)
 *     copy(e)            one entity, by value: optional<T> for a single With,
 *                        optional<tuple<...>> otherwise. Safe to keep across
 *                        structural changes. This is the "optional component" read.
 *     first() / single() first match / the only match (aborts unless exactly one)
 *     contains(e)        does e match the query
 *     has<C...>(e)       does e have C... (any components, tags included)
 *     entities(), entities(subset), count(), empty()
 *     each(f), each(subset, f)    f(Entity, A&, B&); do not add or remove
 *                                 components of the queried types inside f
 *     view()             the raw entt view, as an escape hatch
 *
 * References from atlas / find / get / first / single stay valid until a
 * component of that type is added or removed anywhere.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/commands/primitives.hpp>
#include <moss/commands/command.hpp>

namespace moss::detail {

template<typename... Ts>
struct TypeList { };

template<typename T, typename... Ts>
inline constexpr bool contains_v = (std::is_same_v<T, Ts> || ...);

template<typename Out, typename... In>
struct UniqueImpl {
    using type = Out;
};

template<typename... Out, typename Head, typename... Tail>
struct UniqueImpl<TypeList<Out...>, Head, Tail...> {
    using type = typename UniqueImpl<
        std::conditional_t<contains_v<Head, Out...>, TypeList<Out...>, TypeList<Out..., Head>>,
        Tail...
    >::type;
};

// The distinct types of Ts..., first occurrence wins
template<typename... Ts>
using Unique = typename UniqueImpl<TypeList<>, Ts...>::type;

template<typename... Inc, typename... Ex>
auto makeView(entt::registry& registry, TypeList<Inc...>, TypeList<Ex...>) {
    if constexpr (sizeof...(Ex) == 0) return registry.view<Inc...>();
    else return registry.view<Inc...>(entt::exclude<Ex...>);
}

}

namespace moss::commands {

template<typename... Wth, typename... VwInc, typename... VwEx>
requires(sizeof...(Wth) > 0)
class Query<With<Wth...>, View<Include<VwInc...>, Exclude<VwEx...>>>
    : public Command<Query<With<Wth...>, View<Include<VwInc...>, Exclude<VwEx...>>>> {
public:
    M_SA((!std::is_empty_v<Wth> && ...),
        "Query<With<...>> fetches components, so a tag (empty type) cannot be in With. "
        "Put it in View<Include<...>> or check it with has<T>()");

    using Included = detail::Unique<Wth..., VwInc...>;
    using Excluded = detail::Unique<VwEx...>;

    // What copy() returns: T for a single With, std::tuple<...> otherwise
    using Value = std::conditional_t<
        sizeof...(Wth) == 1,
        std::tuple_element_t<0, std::tuple<Wth...>>,
        std::tuple<Wth...>
    >;

    Query() = default;

    /* ------------------------------ Raw view ------------------------------ */

    [[nodiscard]] auto view() const {
        return detail::makeView(this->registry(), Included{}, Excluded{});
    }

    /* ----------------------------- Many entities ---------------------------- */

    [[nodiscard]] Atlas<Wth...> atlas() const {
        Atlas<Wth...> out;
        for (Entity entity : view()) out.push_back(fetch(entity));
        return out;
    }

    [[nodiscard]] Atlas<Wth...> atlas(const DynamicView& subset) const {
        auto v = view();
        Atlas<Wth...> out;
        out.reserve(subset.size());
        for (Entity entity : subset)
            if (v.contains(entity)) out.push_back(fetch(entity));
        return out;
    }

    template<typename Func>
    void each(Func&& func) const {
        entt::registry& reg = this->registry();
        for (Entity entity : view()) func(entity, reg.get<Wth>(entity)...);
    }

    template<typename Func>
    void each(const DynamicView& subset, Func&& func) const {
        entt::registry& reg = this->registry();
        auto v = view();
        for (Entity entity : subset)
            if (v.contains(entity)) func(entity, reg.get<Wth>(entity)...);
    }

    [[nodiscard]] DynamicView entities() const {
        DynamicView out;
        for (Entity entity : view()) out.push_back(entity);
        return out;
    }

    [[nodiscard]] DynamicView entities(const DynamicView& subset) const {
        auto v = view();
        DynamicView out;
        for (Entity entity : subset)
            if (v.contains(entity)) out.push_back(entity);
        return out;
    }

    [[nodiscard]] usize count() const {
        usize n = 0;
        for (Entity entity : view()) { (void)entity; n++; }
        return n;
    }

    [[nodiscard]] bool empty() const {
        auto v = view();
        return v.begin() == v.end();
    }

    /* ------------------------------ One entity ------------------------------ */

    [[nodiscard]] bool contains(Entity entity) const { return view().contains(entity); }

    template<typename... C>
    [[nodiscard]] bool has(Entity entity) const {
        entt::registry& reg = this->registry();
        return reg.all_of<C...>(entity);
    }

    [[nodiscard]] std::optional<Pool<Wth...>> find(Entity entity) const {
        if (!contains(entity)) return std::nullopt;
        return fetch(entity);
    }

    [[nodiscard]] Pool<Wth...> get(Entity entity) const {
        M_FATAL_IFF(!contains(entity), "Query::get: the entity does not match the query");
        return fetch(entity);
    }

    [[nodiscard]] std::optional<Value> copy(Entity entity) const {
        if (!contains(entity)) return std::nullopt;
        entt::registry& reg = this->registry();
        return Value(reg.get<Wth>(entity)...);
    }

    [[nodiscard]] std::optional<Pool<Wth...>> first() const {
        auto v = view();
        auto it = v.begin();
        if (it == v.end()) return std::nullopt;
        return fetch(*it);
    }

    [[nodiscard]] Pool<Wth...> single() const {
        auto v = view();
        auto it = v.begin();
        M_FATAL_IFF(it == v.end(), "Query::single: no entity matches");
        const Entity entity = *it;
        ++it;
        M_FATAL_IFF(it != v.end(), "Query::single: more than one entity matches");
        return fetch(entity);
    }

private:
    [[nodiscard]] Pool<Wth...> fetch(Entity entity) const {
        entt::registry& reg = this->registry();
        return Pool<Wth...>(reg.get<Wth>(entity)...);
    }
};

}
